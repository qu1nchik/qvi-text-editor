void move_up(Editor *ed);
void move_down(Editor *ed);
void move_right(Editor *ed);
void move_left(Editor *ed);

static void set_cursor(Editor *ed, Line *dest);
static void set_coloff(Editor *ed, Line *dest);

static void set_cursor(Editor *ed, Line *dest) {
  int max = ed->cur->max_x;
  int coloff = ed->viewport->col_offset;

  if (max > dest->length - coloff) {
    if (dest->length > 0) {
      ed->cur->x = dest->length - coloff;
    }

    else {
      ed->cur->x = 1;
    }
  }

  else {
    ed->cur->x = max - coloff;
  }
}

static void set_coloff(Editor *ed, Line *dest) {
  int visible = dest->length - ed->viewport->col_offset;
  int goal;

  // not visible at all(too short)
  if (visible <= 0) {
    goal = dest->length - 5;

    if (goal > 0) {
      ed->viewport->col_offset = goal;
    }

    else {
      ed->viewport->col_offset = 0;
    }
  }

  // not visible fully(too long)
  if (dest->length > ed->viewport->screen_cols - 5 && ed->cur->max_x > ed->viewport->screen_cols - 5) {
    goal = dest->length - (ed->viewport->screen_cols - 5);
    ed->viewport->col_offset = goal;
  }

  // less than 5 chars visible
  if (visible < 5 && visible > 0 && ed->viewport->col_offset > 0) {
    goal = 5 - visible;
    ed->viewport->col_offset -= goal;
  }
}

// UP
void move_up(Editor *ed) {
  int top = ed->viewport->row_offset;

  if (ed->buf->current_line == ed->buf->top) return;

  Line *prev = ed->buf->lines[ed->buf->current_row-2];
  //Moving viewport if necessary
  if (ed->buf->current_row == top + 4 && ed->viewport->row_offset > 0) {
    ed->viewport->row_offset--;
    ed->buf->current_row--;
    set_coloff(ed, prev);
    set_cursor(ed, prev);
    ed->buf->current_line = prev;
    draw_screen(ed);
    return;
  }

  set_coloff(ed, prev);
  set_cursor(ed, prev);
  ed->cur->y--;
  ed->buf->current_row--;
  ed->buf->current_line = prev;
  draw_screen(ed);
}

// DOWN
void move_down(Editor *ed) {
  if (ed->buf->current_line == ed->buf->bot) return;

  int bottom = ed->viewport->screen_rows + ed->viewport->row_offset;
  Line *next = ed->buf->lines[ed->buf->current_row];

  // Moving viewport if necessary
  if (ed->buf->current_row == bottom - 4 && bottom != ed->buf->rows + 1) {
    set_coloff(ed, next);
    set_cursor(ed, next);

    ed->viewport->row_offset++;
    ed->buf->current_row++;
    ed->buf->current_line = next;
    draw_screen(ed);
    return;
  }

  set_coloff(ed, next);
  set_cursor(ed, next);
  ed->cur->y++;
  ed->buf->current_row++;
  ed->buf->current_line = next;
  draw_screen(ed);
}

// RIGHT
void move_right(Editor *ed) {
  int pos = ed->cur->x + ed->viewport->col_offset;
  if (pos >= ed->buf->current_line->length) return;

  // Moving viewport if necessary
  if (ed->cur->x >= ed->viewport->screen_cols - 5) {
    ed->viewport->col_offset++;
    ed->cur->max_x++;
    draw_screen(ed);
    return;
  }

  // Moving the x and max_x values
  ed->cur->x++;
  if (ed->cur->max_x < ed->cur->x + ed->viewport->col_offset) {
    ed->cur->max_x = ed->cur->x + ed->viewport->col_offset;
  }
  draw_screen(ed);
}

// LEFT
void move_left(Editor *ed) {
  if (ed->cur->x == 1) return;

  // Moving screen if necessary
  if (ed->viewport->col_offset > 0 && ed->cur->x == 5) {
    ed->viewport->col_offset--;
    if (ed->cur->max_x >= ed->cur->x + ed->viewport->col_offset) {
      ed->cur->max_x = ed->cur->x + ed->viewport->col_offset;
    }
    draw_screen(ed);
    return;
  }

  // Moving the x + max_x values
  if (ed->cur->max_x > 1) {
    ed->cur->x--;
    if (ed->cur->max_x >= ed->cur->x + ed->viewport->col_offset) {
      ed->cur->max_x = ed->cur->x + ed->viewport->col_offset;
    }
  }
  draw_screen(ed);
}
