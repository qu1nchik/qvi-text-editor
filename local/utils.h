uint8_t Read_Key() {
  uint8_t c;
  int n;

  n = read(0, &c, 1);

  if (n == 1) { return c;
  }

  if (n == -1) {
    perror("read");
    exit(1);
  }
}

void enable_raw_mode() {
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
  printf("%s",CLEAR_AND_GOTO_START);
  printf("You are in alternative buffer!\n");
}

void disable_raw_mode() {
  tcsetattr(0, TCSAFLUSH, &orig_termios);
}
