void normal_mode(Editor *ed) {
  char c;
  enable_raw_mode();
  ed->current_mode = NORMAL;
  draw_screen(ed);

  while(1) {
    // aligning the cursor
    if (ed->cur->x > ed->buf->current_line->length) {
      ed->cur->x = ed->buf->current_line->length;
    }
    if (ed->buf->current_line->length == 0) {
      ed->cur->x = 1;
    }
    c = Read_Key();
    switch (c) {
      // BASIC MOVEMENT:
      case 'j':
        move_down(ed);
        break;

      case 'k':
        move_up(ed);
        break;

      case 'h':
        move_left(ed);
        break;

      case 'l':
        move_right(ed);
        break;

      // Turning on the command mode
      case ':':
        command_mode(ed);
        break;

      // INSERT COMBINATIONS:
      case 'i':
        insert_before(ed);
        break;

      case 'a':
        insert_after(ed);
        break;
      default:
        draw_screen(ed);
        break;
    }
  }
}
