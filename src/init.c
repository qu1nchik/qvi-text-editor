static Line *init_line(const char *str, int len);
static int init_buffer(Buffer *buf, const char *filename);
static void init_empty_buffer(Buffer *buf);

// LINE
static Line *init_line(const char *str, int len) {
  Line *line = malloc(sizeof(Line));
  line->length = len;
  line->capacity = line->length + 4;
  line->data = malloc(line->capacity);

  int i;
  for (i = 0; i < len; i++) {
    line->data[i] = str[i];
  }
  return line;
}
// EMPTY
static void init_empty_buffer(Buffer *buf) {
  Line *line = malloc(sizeof(Line));
  line->length = 0;
  line->capacity = 4;
  line->data = malloc(line->capacity);
 
  buf->lines = malloc(sizeof(Line *));
  buf->lines[0] = line;

  buf->top = buf->lines[0];
  buf->current_line = buf->top;
  buf->bot = buf->top;

  buf->current_row = 1;
  buf->rows = 1;
}

// BUFFER
static int init_buffer(Buffer *buf, const char *filename) {
  struct stat st;

  // if empty or new file
  if (stat(filename, &st) == -1 || st.st_size == 0) {
    init_empty_buffer(buf);
    return 0;
  }

  buf->size = st.st_size;

  char str[1024];
  int i = 0;
  int line_pos = 0;
  int buf_pos;
  int fd = open(filename, O_RDONLY);

  // filling temp buffer
  char temp[buf->size];
  read(fd, temp, buf->size);
  temp[buf->size-1] = '\0';

  //Counting Rows
  buf->rows = 1;
  for (buf_pos = 0; temp[buf_pos] != '\0'; buf_pos++) {
    if (temp[buf_pos] == '\n') {
      buf->rows++;
    }
  }
  buf_pos = 0;

  buf->lines = malloc(sizeof(Line *) * buf->rows);

  // Filling the array
  while (temp[buf_pos] != '\0') {
    if (temp[buf_pos] == '\n') {
      buf->lines[i++] = init_line(str, line_pos);
      line_pos = 0;
      buf_pos++;
    }

    else {
      str[line_pos++] = temp[buf_pos++];
    }
  }
  if (line_pos > 0) {
    buf->lines[i] = init_line(str, line_pos);
  }
  // filling metadata
  buf->current_row = 1;
  buf->top = buf->lines[0];
  buf->current_line = buf->top;
  buf->bot = buf->lines[buf->rows-1];
  return 0;
}

void init_editor(Editor *ed) {
  // initializing Cursor
  ed->cur = malloc(sizeof(Cursor));
  ed->cur->x = 1;
  ed->cur->y = 1;
  ed->cur->max_x = 1;

  // initializing Viewport
  struct winsize ws;

  ioctl(1, TIOCGWINSZ, &ws);

  ed->viewport = malloc(sizeof(Viewport));
  ed->viewport->screen_rows = ws.ws_row;
  ed->viewport->screen_cols = ws.ws_col;
  ed->viewport->row_offset = 0;
  ed->viewport->col_offset = 0;

  //initializing Buffer
  ed->buf = malloc(sizeof(Buffer));
  init_buffer(ed->buf, ed->filename);
}
