uint8_t Read_Key() {
  uint8_t c;
  int n;
  while (1) {
    n = read(0, &c, 1);

    if (n == 1 && c != 0) { 
      return c;
    }

    if (n == -1) {
      perror("read");
      exit(1);
    }
  }
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
  orig_termios.c_lflag |= ECHO;
  orig_termios.c_lflag |= ICANON;
  orig_termios.c_lflag |= ISIG;
  tcsetattr(0, TCSAFLUSH, &orig_termios);
  printf("%s", ALT_BUFFER_OFF);
}

void free_editor(Editor *ed) {
  if (!ed) return;

  printf("%s", CLEAR_AND_GOTO_START);
  fflush(stdout);

  if (ed->buf) {
    if (ed->buf->lines) {
      for (int i = 0; i < ed->buf->rows; i++) {
        if (ed->buf->lines[i]->data) {
          free(ed->buf->lines[i]->data);
          free(ed->buf->lines[i]);
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

static void draw_mode(Editor *ed) {
  // COMMAND
  if (ed->current_mode == COMMAND) {
    printf(":%s", ed->cmd_buf);
    printf("\033[%d;%df", ed->viewport->screen_rows, ed->cmd_pos + 2);
  }

  else {
    // NORMAL
    if (ed->current_mode == NORMAL) {
      if (ed->is_error) {
        printf("Not An Editor Command!");
        ed->is_error = false;
      }
      else {
        printf("--< NORMAL >-- \t\t\t x :%d, len :%d", ed->cur->x, ed->buf->current_line->length);
      }
    }
    // INSERT
    if (ed->current_mode == INSERT) {
      printf("--< INSERT >-- ");
    }

    // Moving the cursor in absolute position and show it
    printf("\033[%d;%df", ed->cur->y, ed->cur->x);
    printf("%s", SHOW_CURSOR);
  }
}

void draw_screen(Editor *ed) {
  printf("%s",CLEAR_AND_GOTO_START);
  int drawn = 0;

  // Normal Drawing
  for (int i = ed->viewport->row_offset; i < ed->viewport->screen_rows - 1 + ed->viewport->row_offset && i < ed->buf->rows; i++) {
    Line *curr = ed->buf->lines[i];
    int len = curr->length;
    int start = ed->viewport->col_offset;
    int chars_left = len - start;

    // current line is longer than gorizontal size of screen
    if (chars_left > ed->viewport->screen_cols) {
      printf("%.*s",ed->viewport->screen_cols, curr->data + start);
      drawn++;
    }

    // shorter
    else {
      // not visible at all
      if (len <= start) {
        printf("\n");
        drawn++;
      }
      // visible by a part or fully
      else {
        printf("%.*s\n", len - start, curr->data + start);
        drawn++;
      }
    }
  }
  for (int i = drawn; i < ed->viewport->screen_rows - 1; i++) {
    printf("\n");
  }
  draw_mode(ed);
  fflush(stdout);
}
