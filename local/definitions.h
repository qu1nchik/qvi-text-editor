// Raw input
#define ALT_BUFFER_ON "\x1b[?1049h"
#define ALT_BUFFER_OFF "\x1b[?1049l"
#define CLEAR_AND_GOTO_START "\x1b[2J\x1b[H"

// ASCII hotkeys(hex)
#define KEY_ESC     0x1B    // ESC = 27
#define KEY_ENTER   0x0D    // Enter (Carriage Return)
#define KEY_TAB     0x09    // Tab
#define KEY_BACKSP  0x7F    // Backspace/Delete = 127
#define KEY_DEL     0x7E    // Delete
#define KEY_CTRL_A  0x01    // Ctrl+A
#define KEY_CTRL_C  0x03    // Ctrl+C
#define KEY_CTRL_D  0x04    // Ctrl+D (EOF)
#define KEY_CTRL_Q  0x11    // Ctrl+Q
#define KEY_CTRL_S  0x13    // Ctrl+S
#define KEY_CTRL_Z  0x1A    // Ctrl+Z

// Escape sequences (hex)
#define ESC_SEQ     0x1B    // Escape
#define CSI         0x5B    // '[' после ESC
#define ARROW_UP    0x41    // 'A'
#define ARROW_DOWN  0x42    // 'B'
#define ARROW_RIGHT 0x43    // 'C'
#define ARROW_LEFT  0x44    // 'D'

// Functions

void enable_raw_mode(void);
void disable_raw_mode(void);
uint8_t Read_Key(void);
void init_line(const char *str, Line *line);
int init_buffer(Buffer *buf, const char *filename);
void move_up(Buffer *buf);
void move_down(Buffer *buf);
void show_current(Buffer *buf);
