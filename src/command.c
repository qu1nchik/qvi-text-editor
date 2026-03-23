enum Command {
  CMD_WRITE,
  CMD_QUIT
};
typedef void (*CmdFunc)(Editor *);

static void cmd_write(Editor *ed);
static void cmd_quit(Editor *ed);
static void cmd_parse(Editor *ed);
static void cmd_execute(Editor *ed, CmdFunc *manual);
static bool is_valid(char c);

CmdFunc cmd_table[] = {
  [CMD_WRITE] = cmd_write, 
  [CMD_QUIT] = cmd_quit
};
void command_mode(Editor *ed) {
  ed->current_mode = COMMAND;
  int c;
  while (1) {
    c = Read_Key();
    if (c != KEY_CTRL_C && c != KEY_ESC) {
      switch (c) {
      case KEY_BACKSP:
        if (ed->cmd_pos == 0) {
          normal_mode(ed);
        } else {
          ed->cmd_buf[--ed->cmd_pos] = '\0';
        }
        draw_screen(ed);
        break;

      case KEY_ENTER:
        cmd_parse(ed);
        normal_mode(ed);
        break;

      default:
        if (c != 0) {
          ed->cmd_buf[ed->cmd_pos++] = c;
          ed->cmd_buf[ed->cmd_pos] = '\0';
        }
        draw_screen(ed);
      }
    }

    else {
      normal_mode(ed);
      return;
    }
  }
}

static void cmd_parse(Editor *ed) {
  CmdFunc cmd_man[8];
  int counter = 0;
  bool is_filename_changed = false;
  int pos;

  for (pos = 0; ed->cmd_buf[pos] != '\0'; pos++) {
    char c = ed->cmd_buf[pos];

    // Write combos
    if (c == 'w') {

      // if :w *filename* format
      if (ed->cmd_buf[pos + 1] == ' ' && ed->cmd_buf[pos + 2] != '\0') {
        ed->filename = strdup(&ed->cmd_buf[pos + 2]);
        is_filename_changed = true;
      }

      cmd_man[counter++] = cmd_table[CMD_WRITE];
    }

    if (!is_valid(c)) {
      if (is_filename_changed) {
        continue;
      }

      else {
        goto fatal;
      }
    }

    // Quit combos
    if (c == 'q') {
      // if 'q' isn`t the last char
      if (ed->cmd_buf[pos + 1] != '\0')
        goto fatal;

      else {
        cmd_man[counter++] = cmd_table[CMD_QUIT];
      }
    }
  }
  cmd_man[counter] = NULL;
  cmd_execute(ed, cmd_man);
  ed->cmd_buf[0] = '\0';
  ed->cmd_pos = 0;
  return;

fatal:
  ed->is_error = true;
  ed->cmd_buf[0] = '\0';
  ed->cmd_pos = 0;
  return;
}

static void cmd_execute(Editor *ed, CmdFunc *manual) {
  for (int i = 0; manual[i]; i++) {
    manual[i](ed);
  }
}

static void cmd_write(Editor *ed) {
  int fd = open(ed->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  char text[ed->buf->size];
  int i;
  int j;
  int pos = 0;

  for (i = 0; i < ed->buf->rows; i++) {
    for (j = 0; ed->buf->lines[i].data[j] != '\0'; j++) {
      text[pos++] = ed->buf->lines[i].data[j];
    }
  }

  write(fd, text, pos);
  close(fd);
}

static void cmd_quit(Editor *ed) {
  disable_raw_mode();
  free_editor(ed);
  exit(0);
}

static bool is_valid(char c) {
  char valid_chars[2] = {'w', 'q'};
  int res;
  for (int i = 0; i < sizeof(valid_chars) / sizeof(char); i++) {
    if (valid_chars[i] == c) {
      return true;
    }
  }
  return false;
}
