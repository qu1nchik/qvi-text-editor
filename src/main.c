#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

typedef struct {
  char *data;
    // pointer to the line
  
  int length;
    // pure length,hello - 5
  
  size_t size;
    // the current memory size for line including \n and \0,hello - 7 because of the hello\n\0
  
  size_t capacity;
    // quantity of current allocated memory for line, equals size * 2 when size == capacity - 1
  
} Line;

typedef struct {
  Line *lines;
    // Lines array
  
  Line *current_line;
    // Pointer to the current line in the text
  
  size_t size;
    // Size of file(before the changes)
  
  int current_row;
    // Position of the current row in the text
  
  int rows;
    // Rows quantity

} Buffer;

typedef struct {
  int x;
    // X position of cursor

  int y;
    // Y position of cursor
  
  int max_x;
    /* Last maximum value of x,for example hello\n\0 - length = 5, and the next line is qvi\n\0 - length 3,
       so we moved on to x = 5 y = 1 and ("o" in hello) and at this point this value equal 5,we pressed down
       and here`s the important moment - the cursor x is 3 now ("i" in qvi) cuz max > current_line->length(3 for qvi), but the
       max value is still remembered as 5, this means that if we are going to move on next line which length >= max_x
       (for example "world") we will be on the 5 position relatively x.
       Thats basically Vim/Neovim feature(and yup i stoled this, idc btw :>) */

} Cursor;

typedef struct {
  int screen_rows;
    // Rows of Viewport

  int screen_cols;
    // Cols of Viewport

  int row_offset;
    // First visible line
  
  int col_offset;
    // First visible col
  
} Viewport;

typedef struct {
  char *filename;
  int current_mode;
  bool is_new;

  //Modules of the editor
  Buffer *buf; 
  Cursor *cur;
  Viewport *viewport;
} Editor;

struct termios orig_termios;
struct termios new_termios;

#include "../local/definitions.h"

int main(int argc, char **argv) {
  Editor Qvi;
  int fd;
  if (argc == 2) {
    if ((fd = open(argv[1], O_RDONLY)) == -1 ) {
      fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
    }

    Qvi.filename = argv[1];
  } 
  else {
    printf("usage: ./qvi *file*\n");
    return 1;
  }
  close(fd);

  init_editor(&Qvi);

  normal_mode(&Qvi);
  return 0;
}
