// LINE
void init_line(const char *str, Line *line) {
  int i;
  for (i = 0; str[i] != '\0' ;i++) {
    // empty, just counting
  }

  line->length = i;
  line->size = i + 2;
  line->capacity = line->size * 2;
  line->data = malloc(line->capacity); 
  
  for (i = 0; i < line->length ;i++) {
    line->data[i] = str[i];
  }
  line->data[i++] = '\n';
  line->data[i] = '\0';

}

// BUFFER
int init_buffer(Buffer *buf, const char *filename) {
  //Getting size of file in bytes
  struct stat st;
  if (stat(filename, &st) == -1 ) return -1;
  buf->size = st.st_size;
  
  char str[1024];
  int i = 0;
  int buf_pos = 0;
  int line_pos = 0;
  int fd = open(filename, O_RDONLY);
  
  if (fd == -1) {
    perror("open");
    return -1;
  }

  //Allocating memory for n bytes + \0 to bufferize the temp buffer
  char *temp = malloc(buf->size + 1); //

  read(fd, temp, buf->size);
  temp[buf->size] = '\0';
  
  //Counting Rows
  buf->rows = 0;

  while (temp[buf_pos] != '\0') {
    if (temp[buf_pos] == '\n') {
      buf->rows++;
      buf_pos++;
    }

    else {
      buf_pos++;
    }
  }
  buf_pos = 0;

  //Allocating memory for array of structures
  buf->lines = malloc(sizeof(Line) * buf->rows);

  // Filling the array
  while (temp[buf_pos] != '\0') {
    if (temp[buf_pos] == '\n') {
      str[line_pos] = '\0';
      init_line(str, &buf->lines[i]);
      line_pos = 0;
      buf_pos++;
      i++;
    }
    
    else {
      str[line_pos++] = temp[buf_pos++];
    }
  }

  if (line_pos > 0) {
    str[line_pos] = '\0';
    init_line(str, &buf->lines[i]);
  }
  // filling metadata
  buf->current_row = 1;
  buf->current_line = buf->lines;

  return 0;
}

void init_editor(Editor *ed) {

  // initializing Cursor
  ed->cur = malloc(sizeof(Cursor));
  ed->cur->x = 1;
  ed->cur->y = 1;
  ed->cur->max_x = 1;

  // initializing Viewport
  struct winsize ws;  
  
  ioctl(1, TIOCGWINSZ, &ws);

  ed->viewport = malloc(sizeof(Viewport));
  ed->viewport->screen_rows = ws.ws_row;
  ed->viewport->screen_cols = ws.ws_col;
  ed->viewport->row_offset = 0;
  ed->viewport->col_offset = 0;

  //initializing Buffer
  ed->buf = malloc(sizeof(Buffer));
  init_buffer(ed->buf, ed->filename);
}
