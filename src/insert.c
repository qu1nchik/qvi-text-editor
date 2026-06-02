// inner functions
static void insert_mode(Editor *ed, int pos);
static void insert_char(Editor *ed, int *pos, char c);
static void delete_char(Editor *ed, int *pos);
static void split_line(Editor *ed);
static void merge_line(Editor *ed);

// global instuction functions(for normal_mode)
void insert_before(Editor *ed);
void insert_after(Editor *ed);


static void insert_mode(Editor *ed, int pos) {
  ed->current_mode = INSERT;
  char c;
  draw_screen(ed);

  while(1) {
    c = Read_Key();
    if (c == KEY_CTRL_C || c == KEY_ESC) return;

    switch(c) {
      // deleting a character(backspace case)
      case KEY_BACKSP:
        delete_char(ed, &pos);
        break;

      // splitting line('\n' char case)
      case KEY_ENTER:
        break;

      // regular insertion of a char(not a newline or backspace)
      default:
        insert_char(ed, &pos, c);
        break;
    }
  }
}

static void insert_char(Editor *ed, int *pos, char c) {
  int size = ed->buf->current_line->length + 2;

  // reallocating memory if necessary
  if (size >= ed->buf->current_line->capacity - 1) {
    ed->buf->current_line->capacity += 16;
    ed->buf->current_line->data = realloc(ed->buf->current_line->data, ed->buf->current_line->capacity);
  }

  // Moving memory
  memmove((ed->buf->current_line->data + 1) + (*pos),
          ed->buf->current_line->data + (*pos),
          ed->buf->current_line->length - (*pos));
  // inserting and incrementing
  ed->buf->current_line->data[(*pos)++] = c;
  ed->buf->current_line->length++;
  ed->cur->x++;
  draw_screen(ed);
}

static void delete_char(Editor *ed, int *pos) {
  if (ed->cur->y == 1 && ed->cur->x == 1) return;

  // if the cursor x position at the beginning of the line
  if (ed->cur->x == 1) {
    merge_line(ed);
  }

  int excess_mem = ed->buf->current_line->capacity - 16;
  int size = ed->buf->current_line->length;
  // Reallocating memory if necessary
  if (size < excess_mem) {
    ed->buf->current_line->capacity -= 16;
    ed->buf->current_line->data = realloc(ed->buf->current_line->data, ed->buf->current_line->capacity);
  }
  // Moving memory
  memmove(ed->buf->current_line->data + (*pos) - 1, // dest
          ed->buf->current_line->data + (*pos),    // src
          ed->buf->current_line->length - (*pos)); // n
  (*pos)--;
  ed->buf->current_line->length--;
  ed->cur->x--;
  draw_screen(ed);
}

static void split_line(Editor *ed) {

}

static void merge_line(Editor *ed) {
  Line *curr = ed->buf->current_line;
  Line *prev = ed->buf->lines[ed->buf->current_row-2];
  int len = prev->length;

  prev->length += curr->length;

  // reallocating memory if necessary
  if (prev->length >= prev->capacity) {
    prev->capacity += (prev->length) - prev->capacity;
    prev->data = realloc(prev->data, prev->capacity);
  }

  // merging strings
  for (int i = len; i-len < curr->length; i++) {
    prev->data[i] = curr->data[i-len];
  }

  // removing line(by moving memory)
  memmove(ed->buf->lines+ed->buf->current_row-1,
          ed->buf->lines+ed->buf->current_row,
          sizeof(Line *) * (ed->buf->rows - ed->buf->current_row));
  free(ed->buf->current_line);
  free(ed->buf->current_line->data);

  ed->buf->lines = realloc(ed->buf->lines, sizeof(Line *) * --ed->buf->rows);
  // updating buffer and cursor data
  ed->buf->current_line = prev;
  ed->buf->current_row--;
  ed->cur->y--;
  ed->cur->x = len + 1;
}

void insert_after(Editor *ed) {
  if (ed->buf->current_line->length == 0) insert_before(ed);
  int pos = ed->cur->x + ed->viewport->col_offset;
  ed->cur->x++;
  insert_mode(ed, pos);
  ed->cur->x--;
  normal_mode(ed);
}

void insert_before(Editor *ed) {
  int pos = (ed->cur->x + ed->viewport->col_offset) - 1;
  insert_mode(ed, pos);
  normal_mode(ed);
}
