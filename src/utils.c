uint8_t Read_Key() {
  uint8_t c;
  int n;

  n = read(0, &c, 1);

  if (n == 1) { 
    return c;
  }

  if (n == -1) {
    perror("read");
    exit(1);
  }
  return 0;
}

void enable_raw_mode() {
  tcgetattr(0, &orig_termios);
  new_termios = orig_termios;
  tcgetattr(0, &new_termios);
  // setting up the flags
  new_termios.c_lflag &= ~(ICANON); 
  new_termios.c_lflag &= ~(ECHO); 
  new_termios.c_lflag &= ~(ISIG); 
  new_termios.c_cc[VMIN] = 0;
  new_termios.c_cc[VTIME] = 1;

  tcsetattr(0, TCSAFLUSH, &new_termios);

  // turning on the alt buffer
  printf("%s",ALT_BUFFER_ON);
  printf("%s",GOTO_START);
}

void disable_raw_mode() {
  tcsetattr(0, TCSAFLUSH, &orig_termios);
}

void free_editor(Editor *ed) {
  if (!ed) return;
  
  printf("%s", CLEAR_AND_GOTO_START);
  fflush(stdout);
  
  if (ed->buf) {
    if (ed->buf->lines) {
      for (int i = 0; i < ed->buf->rows; i++) {
        if (ed->buf->lines[i].data) {
          free(ed->buf->lines[i].data);
        }
      }

      free(ed->buf->lines);
      ed->buf->lines = NULL;
    }
    free(ed->buf);
    ed->buf = NULL;
  }

  if (ed->cur) {
    free(ed->cur);
    ed->cur = NULL;
  }
  
  if (ed->viewport) {
    free(ed->viewport);
    ed->viewport = NULL;
  }
}

void draw_screen(Editor *ed) {

  printf("%s",CLEAR_AND_GOTO_START);

  // Drawing
  int i;
  int max_line = ed->buf->rows;
  int lines_needed_tilde = ed->viewport->screen_rows - ed->buf->rows - 1;

  // Normal Drawing
  if (ed->viewport->screen_rows < ed->buf->rows) {
    for (i = ed->viewport->row_offset; i < ed->viewport->screen_rows - 1 + ed->viewport->row_offset && i < max_line; i++) {
      int len = ed->buf->lines[i].length;
      int start = ed->viewport->col_offset;
      int chars_left = len - start;

      // if longer than viewport
      if (chars_left > ed->viewport->screen_cols) {
        printf("%.*s",ed->viewport->screen_cols, ed->buf->lines[i].data + ed->viewport->col_offset);
      }
      
      // if shorter than viewport
      else {
        if (len <= ed->viewport->col_offset) {
          printf("\n");
        }

        else {
          printf("%s", ed->buf->lines[i].data + ed->viewport->col_offset);
        }
      }
    }
  }
  //Drawing with tildes(if size of file is less than size of viewport)
  else {

    // Drawing data
    for (i = 0; i < ed->buf->rows; i++) {
      printf("%s", ed->buf->lines[i].data);
    }
    
    // Drawing tildes
    for (i = 0; i < lines_needed_tilde; i++) {
      if (ed->cur->x - 1 == i) {
        printf("\n");
      }

      else {
        printf("~\n");
      }
    }
  }

  if (ed->current_mode == NORMAL) {
    printf("--< NORMAL >-- \t\t\t\t\t\t max: %d, offset: %d, x: %d", ed->cur->max_x, ed->viewport->col_offset, ed->cur->x);
  }
  
  if (ed->current_mode == INSERT) {
    printf("--< INSERT >-- \t\t\t\t\t\t %d, %d, %d ", ed->buf->current_line->length, ed->buf->current_line->size, ed->buf->current_line->capacity);
  }
  // Moving the cursor in absolute position and show it
  printf("\033[%d;%df", ed->cur->y, ed->cur->x);
  printf("%s", SHOW_CURSOR);
  
  fflush(stdout);
}
