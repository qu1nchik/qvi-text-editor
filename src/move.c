// UP
void move_up(Editor *ed) {
  int top = ed->viewport->row_offset;

  if (ed->buf->current_row == 1) return;

  Line *prev = ed->buf->current_line - 1;
  //Moving viewport if need
  if (ed->buf->current_row == top + 4 && ed->viewport->row_offset > 0) {
    ed->viewport->row_offset--;
    ed->buf->current_row--;
    ed->buf->current_line--;

    if (ed->cur->max_x > prev->length) {
      ed->cur->x = prev->length;
    }

    else {
      ed->cur->x = ed->cur->max_x - ed->viewport->col_offset;
    }

    draw_screen(ed);
    return;
  }
  // ADVANCED MOVEMENT CONDITIONS(important block of code in vertical up movement)
  int visible = prev->length - ed->viewport->col_offset;  
  int goal;

  //1. if next is not visible at all(cuz its shorter)
  if (visible < 0) {
    goal = prev->length - 5;

    if (goal > 0) {
      ed->viewport->col_offset = goal;
    }

    else {
      ed->viewport->col_offset = 0;
    }
  }
  
  // 2. if next is not visible fully (cuz its longer)
  if (prev->length > ed->viewport->screen_cols - 5 && ed->cur->max_x > ed->viewport->screen_cols - 5) {
    goal = prev->length - (ed->viewport->screen_cols - 5);
    ed->viewport->col_offset = goal;
  }

  //3. if there is only 4 or less visible chars
  if (visible < 5 && visible > 0 && ed->viewport->col_offset > 0) {
    goal = 5 - visible;
    ed->viewport->col_offset -= goal;
  }

  // Moving Up algo(Description in main.c at the typedef struct Cursor comments)
  if (ed->cur->max_x > prev->length - ed->viewport->col_offset) {
    ed->cur->x = prev->length - ed->viewport->col_offset;
  }

  else {
    ed->cur->x = ed->cur->max_x - ed->viewport->col_offset;
  }

  ed->cur->y--;
  ed->buf->current_row--;
  ed->buf->current_line--;
  
  draw_screen(ed);
}

// DOWN 
void move_down(Editor *ed) {
  int bottom = ed->viewport->screen_rows + ed->viewport->row_offset;
  Line *next = ed->buf->current_line + 1;
  if (ed->buf->current_row >= ed->buf->rows) return;

  // Moving viewport if need
  if (ed->buf->current_row == bottom - 4 && bottom != ed->buf->rows + 1) {

    ed->viewport->row_offset++;
    ed->buf->current_row++;
    ed->buf->current_line++;

    if (ed->cur->max_x > next->length) {
      ed->cur->x = next->length;
    }

    else {
      ed->cur->x = ed->cur->max_x - ed->viewport->col_offset;
    }

    draw_screen(ed);
    return;
  }
  // ADVANCED MOVEMENT CONDITIONS(important block of code in vertical down movement)
  int visible = next->length - ed->viewport->col_offset;
  int goal;

  //1. if next is not visible at all(cuz its shorter)
  if (visible < 0) {
    goal = next->length - 5;

    if (goal > 0) {
      ed->viewport->col_offset = goal;
    }

    else {
      ed->viewport->col_offset = 0;
    }
  }
  
  // 2. if next is not visible fully (cuz its longer)
  if (next->length > ed->viewport->screen_cols - 5 && ed->cur->max_x > ed->viewport->screen_cols - 5) {
    goal = next->length - (ed->viewport->screen_cols - 5);
    ed->viewport->col_offset = goal;
  }

  //3. if there is only 4 or less visible chars
  if (visible < 5 && visible > 0 && ed->viewport->col_offset > 0) {
    goal = 5 - visible;
    ed->viewport->col_offset -= goal;
  }

  // Moving Down algo(Description in main.c at the typedef struct Cursor comments)
  if (ed->cur->max_x > next->length - ed->viewport->col_offset) {
    ed->cur->x = next->length - ed->viewport->col_offset;
  }

  else {
    ed->cur->x = ed->cur->max_x - ed->viewport->col_offset;
  }
  
  ed->cur->y++;
  ed->buf->current_row++;
  ed->buf->current_line++;

  draw_screen(ed);
}

// RIGHT
void move_right(Editor *ed) {
  // Moving screen if need
  if (ed->cur->x + ed->viewport->col_offset < ed->buf->current_line->length) {
    if (ed->cur->x >= ed->viewport->screen_cols - 5) {
      ed->viewport->col_offset++;
      ed->cur->max_x++;
      draw_screen(ed);
      return;
    }

  // Moving the x + max_x values
    ed->cur->x++;

    if (ed->cur->max_x < ed->cur->x) {
      ed->cur->max_x = ed->cur->x + ed->viewport->col_offset;
    }
  }
  draw_screen(ed);
}

// LEFT
void move_left(Editor *ed) {
  // Moving screen if need
  if (ed->viewport->col_offset > 0 && ed->cur->x - 4 == 1) {
    ed->viewport->col_offset--;
    ed->cur->max_x--;
    draw_screen(ed);
    return;
  }

  // Moving the x + max_x values
  if (ed->cur->x > 1 && ed->cur->max_x > 1) {
    ed->cur->x--;

    if (ed->cur->max_x > ed->cur->x) {
      ed->cur->max_x = ed->cur->x + ed->viewport->col_offset; // max_x considering the current gorizontal offset 
    }
  }
  draw_screen(ed);
}
