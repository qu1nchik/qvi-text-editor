#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct {
  char *data;
  int length;
  size_t capacity;
} Line;

typedef struct {
  Line *lines;
  Line *current_line;
  size_t size;
  int current_row;
  int rows;
} Buffer;

typedef struct {
  char *filename;
  Buffer *buf;
} Editor;

struct termios orig_termios;
struct termios new_termios;

#include "../local/definitions.h"
#include "../local/utils.h"


void init_line(const char *str, Line *line) {
  int i;
  for (i = 0; str[i] != '\n' && str[i] != '\0' ;i++) {
    // empty, just counting
  }
  
  line->length = i; // len without \0, pure metadata length, for example: "hello" it`s 5 not including \0
  line->capacity = i + 1; // memory for line, including \0, for example: "hello\0" 6 bytes

  line->data = malloc(line->capacity); 
  
  for (i = 0; i < line->length ;i++) {
    line->data[i] = str[i];
  }
  line->data[i] = '\0';
}

int init_buffer(Buffer *buf, const char *filename) {
  //Getting size of file in bytes
  struct stat st;
  if (stat(filename, &st) == -1 ) return -1;
  buf->size = st.st_size;
  
  char str[1024];
  Line temp_line;
  int i = 0;
  int buf_pos = 0;
  int line_pos = 0;
  int fd = open(filename, O_RDONLY);
  
  if (fd == -1) {
    perror("open");
    return -1;
  }

  //Allocating memory for n bytes + \0 to bufferize the temp buffer
  char *temp = malloc(buf->size + 1);

  read(fd, temp, buf->size);
  temp[buf->size] = '\0';
  
  //Counting Rows
  while (temp[buf_pos] != '\0') {
    if (temp[buf_pos] == '\n') {
      buf->rows++;
      buf_pos++;
    }
    buf_pos++;
  }
  buf->rows++;
  buf_pos = 0;

  //Allocating memory for array of structures
  buf->lines = malloc(sizeof(Line) * buf->rows);

  // Filling the array
  while (temp[buf_pos] != '\0') {
    if (temp[buf_pos] == '\n') {
      str[line_pos] = '\n';
      init_line(str, &temp_line);
      buf->lines[i] = temp_line;
      line_pos = 0;
      buf_pos++;
      i++;
    }
    str[line_pos++] = temp[buf_pos++];
  }
  if (temp[buf_pos] == '\0') {
    str[line_pos] = '\n';
    init_line(str, &temp_line);
    buf->lines[i] = temp_line;
  }
  // Free the temp buffer to avoid segfault`s
  free(temp_line.data);
  buf->current_row = 0;
  buf->current_line = buf->lines;
  return 0;
}

void move_up(Buffer *buf) {
  if (buf->current_row != 0) {
    buf->current_line = &buf->lines[--buf->current_row];
  }
}

void move_down(Buffer *buf) {
  if (buf->current_row != buf->rows - 1) {
    buf->current_line = &buf->lines[++buf->current_row];
  }
}

void show_current(Buffer *buf) {
  printf("%s\n", buf->current_line->data);
}

int main(int argc, char **argv) {
  char c;
  Editor Qvi;
  if (argc == 2) {
    Qvi.filename = argv[1];
  } 
  else {
    printf("usage: ./qvi *file*\n");
    return 1;
  }

  Qvi.buf = malloc(sizeof(Buffer));
  init_buffer(Qvi.buf, Qvi.filename);

  tcgetattr(0, &orig_termios);
  enable_raw_mode();

  while (1) {
    c = Read_Key();
    switch (c) {
      case 'k':
        move_up(Qvi.buf);
        break;

      case 'j':
        move_down(Qvi.buf);
        break;

      case 'p':
        show_current(Qvi.buf);
        break;

      case KEY_CTRL_C:
        disable_raw_mode();
        break;
      default:
        if (c != 0) {
          printf("%d\n", c);
        }
    }
  }
  return 0;
}

void free_buffer(Buffer *buf) {
  int i;
  for (i = 0; i < buf->rows;i++) {
    free(buf->lines[i].data);
  }
  free(buf->lines);
  buf->current_line = NULL;
}
