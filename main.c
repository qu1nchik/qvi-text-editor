#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "definitions.h"

typedef struct {
  char *data;
  int length;
  int capacity;
} Line;

typedef struct {
  Line *lines;
  int rows;
} Buffer;

struct termios orig_termios;
struct termios new_termios;

int init_line(const char *input_line, Line *line) {
  int i;
  for (i = 0; input_line[i] != '\n'; i++) {
    line->data[i] = input_line[i];
    line->length++;
  }
  line->data[i] = input_line[i];
  line->length++;
  return 0;
}
int init_buffer(int fd, Buffer buf) {
  int i;
}

int main(int argc, char **argv) {
  char c;
  tcgetattr(0, &orig_termios);
  enable_raw_mode();
  while (1) {
    c = Read_Key();
    if (c != 0) {
      printf("%d\n", c);
    }
    if (c == KEY_CTRL_C) {
      disable_raw_mode();
      break;
    }
  }
  return 0;
}

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
