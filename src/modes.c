void normal_mode(Editor *ed) {
  char c;
  enable_raw_mode();
  ed->current_mode = NORMAL;
  draw_screen(ed);

  while(1) {
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
      
      // INSERT COMBINATIONS:
      case 'q':
        free_editor(ed);
        disable_raw_mode();
        return;
    }
  }
}
