// Alt buffer stuff 
#define ALT_BUFFER_ON "\033[?1049h"
#define ALT_BUFFER_OFF "\033[?1049l"
#define CLEAR_AND_GOTO_START "\033[2J\033[H"
#define GOTO_START "\033[H"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"


// ASCII hotkeys(hex)
#define KEY_ESC     0x1B    // ESC = 27
#define KEY_ENTER   0x0A    // Enter (Carriage Return) \n
#define KEY_TAB     0x09    // Tab
#define KEY_BACKSP  0x7F    // Backspace/Delete = 127
#define KEY_DEL     0x7E    // Delete
#define KEY_CTRL_A  0x01    // Ctrl+A
#define KEY_CTRL_C  0x03    // Ctrl+C
#define KEY_CTRL_D  0x04    // Ctrl+D (EOF)
#define KEY_CTRL_Q  0x11    // Ctrl+Q
#define KEY_CTRL_S  0x13    // Ctrl+S
#define KEY_CTRL_Z  0x1A    // Ctrl+Z

// Modes macro
#define NORMAL 1
#define INSERT 2
#define COMMAND 3

// FUNCTIONS
void init_editor(Editor *ed);

// Utils
void enable_raw_mode(void);
void disable_raw_mode(void);
uint8_t Read_Key(void);
void free_editor(Editor *ed);
void draw_screen(Editor *ed);


// Movement
void move_up(Editor *ed);
void move_down(Editor *ed);
void move_right(Editor *ed);
void move_left(Editor *ed);


// Modes
void normal_mode(Editor *ed);
void command_mode(Editor *ed);

// Insert Functions
void insert_after(Editor *ed);
void insert_before(Editor *ed);

// Inclusions
#include "../src/init.c"
#include "../src/utils.c"
#include "../src/move.c"
#include "../src/normal.c"
#include "../src/command.c"
#include "../src/insert.c"
