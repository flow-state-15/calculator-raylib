#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define APP_NAME "Simple Calculator"

static bool wasFocused = true;

typedef char **InputBuffer[256];

struct AppState {
  InputBuffer in_buf;
  int result;
};

void sleep_program(int ms) {
#ifdef _WIN32
  Sleep(ms);

#else
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&ts, NULL);
#endif
}

void DrawSquare(int posX, int posY, int size, Color color, char *text) {
  DrawRectangle(posX, posY, size, size, color);
  DrawText(text, posX + 20, posY + 17, 20, DARKGRAY);
}

void DrawDSquare(int posX, int posY, int size, Color color, char *text) {
  DrawRectangle(posX, posY, size * 2, size, color);
  DrawText(text, posX + 20, posY + 34, 20, DARKGRAY);
}

void init_window() {
  // init window
  InitWindow(208, 327, APP_NAME);
  SetTargetFPS(15);
}

typedef struct {
  Rectangle rect;
  char *tkn;
  Color color;
  bool is_square;
} Button;

#define NUMBER_OF_BUTTONS 18

static void print_buttons(Button btns[NUMBER_OF_BUTTONS]) {
  size_t n = 0;
  while (n < NUMBER_OF_BUTTONS) {
    printf("Button token: %s\n", btns[n].tkn);
    n++;
  }
}

static Button
make_btn(int x, int y, int sz, Color c, char *tkn, bool is_square)
{
  Rectangle r = {.x = (float) x, .y = (float) y,
                 .width = (float) sz, .height = (float) sz};
  return (Button) {.rect = r, .color = c, .tkn = tkn, .is_square = is_square};
}

void init_button_geo(Button btns[NUMBER_OF_BUTTONS]) {

    /* int topOffset = 88 + 4;  // Height of top areas + edge padding */

#if 1
    // Row and column setup
    int rowHeight = 50;
    int colWidth = 50;
    
    // Calculate the start positions (x, y) for the first row at the bottom
    int startX = 4;  // Edge padding
    int startY = 327 - 2 - 49;  // Bottom edge padding and one button height
    

    btns[0] = make_btn(startX, startY, 49, LIGHTGRAY, "C", true);
    btns[1] = make_btn(startX + colWidth, startY, 49, LIGHTGRAY, "=", true);
    btns[2] = make_btn(startX + (colWidth * 3), startY - rowHeight,
             49, LIGHTGRAY, "/", false);
    btns[3] = make_btn(startX + (colWidth * 3), startY - 2 * rowHeight,
             49, LIGHTGRAY, "*", false);
    btns[4] = make_btn(startX + (colWidth * 3), startY - 3 * rowHeight,
             49, LIGHTGRAY, "-", false);
    btns[5] = make_btn(startX + (colWidth * 3), startY - 4 * rowHeight,
             49, LIGHTGRAY, "+", false);
    btns[6] = make_btn(startX + colWidth, startY - rowHeight,
             49, LIGHTGRAY, "(", false);
    btns[7] = make_btn(startX + (colWidth * 2), startY - rowHeight,
             49, LIGHTGRAY, ")", false);
    btns[8] = make_btn(startX + colWidth, startY - rowHeight,
             49, LIGHTGRAY, "0", false);
    btns[9] = make_btn(startX, startY - 2 * rowHeight,
             49, LIGHTGRAY, "1", false);
    btns[10] = make_btn(startX + colWidth, startY - 2 * rowHeight,
             49, LIGHTGRAY, "2", false);
    btns[11] = make_btn(startX + 2 * colWidth, startY - 2 * rowHeight,
             49, LIGHTGRAY, "3", false);
    btns[12] = make_btn(startX, startY - 3 * rowHeight,
             49, LIGHTGRAY, "4", false);
    btns[13] = make_btn(startX + colWidth, startY - 3 * rowHeight,
             49, LIGHTGRAY, "5", false);
    btns[14] = make_btn(startX + 2 * colWidth, startY - 3 * rowHeight,
             49, LIGHTGRAY, "6", false);
    btns[15] = make_btn(startX, startY - 4 * rowHeight,
             49, LIGHTGRAY, "7", false);
    btns[16] = make_btn(startX + colWidth, startY - 4 * rowHeight,
             49, LIGHTGRAY, "8", false);
    btns[17] = make_btn(startX + (colWidth * 2), startY - 4 * rowHeight,
             49, LIGHTGRAY, "9", false);
#endif
}

/* void draw_button_grid(Button *buttons) { */
/*     // Operator buttons on the right */
/*     Vector2 divide_button = DrawSquare(startX + (colWidth * 3), startY - rowHeight, 49, LIGHTGRAY, "/"); */
/*     Vector2 multiply_button = DrawSquare(startX + (colWidth * 3), startY - 2 * rowHeight, 49, LIGHTGRAY, "*"); */
/*     Vector2 minus_button = DrawSquare(startX + (colWidth * 3), startY - 3 * rowHeight, 49, LIGHTGRAY, "-"); */
/*     Vector2 plus_button = DrawSquare(startX + (colWidth * 3), startY - 4 * rowHeight, 49, LIGHTGRAY, "+"); */
    
/*     // Special function buttons */
/*     Vector2 clear_button = DrawDSquare(startX, startY, 49, LIGHTGRAY, "C"); */
/*     Vector2 equals_button = DrawDSquare(startX + colWidth, startY, 49, LIGHTGRAY, "="); */
/*     Vector2 open_paren_button = DrawSquare(startX + colWidth, startY - rowHeight, 49, LIGHTGRAY, "("); */
/*     Vector2 close_paren_button = DrawSquare(startX + (colWidth * 2), startY - rowHeight, 49, LIGHTGRAY, ")"); */

/*     // Number buttons */
/*     Vector2 zero_button = DrawSquare(startX + colWidth, startY - rowHeight, 49, LIGHTGRAY, "0"); */
/*     Vector2 one_button = DrawSquare(startX, startY - 2 * rowHeight, 49, LIGHTGRAY, "1"); */
/*     Vector2 two_button = DrawSquare(startX + colWidth, startY - 2 * rowHeight, 49, LIGHTGRAY, "2"); */
/*     Vector2 three_button = DrawSquare(startX + 2 * colWidth, startY - 2 * rowHeight, 49, LIGHTGRAY, "3"); */
/*     Vector2 four_button = DrawSquare(startX, startY - 3 * rowHeight, 49, LIGHTGRAY, "4"); */
/*     Vector2 five_button = DrawSquare(startX + colWidth, startY - 3 * rowHeight, 49, LIGHTGRAY, "5"); */
/*     Vector2 six_button = DrawSquare(startX + 2 * colWidth, startY - 3 * rowHeight, 49, LIGHTGRAY, "6"); */
/*     Vector2 seven_button = DrawSquare(startX, startY - 4 * rowHeight, 49, LIGHTGRAY, "7"); */
/*     Vector2 eight_button = DrawSquare(startX + colWidth, startY - 4 * rowHeight, 49, LIGHTGRAY, "8"); */
/*     Vector2 nine_button = DrawSquare(startX + (colWidth * 2), startY - 4 * rowHeight, 49, LIGHTGRAY, "9"); */
/* } */


int main(void) {
  Button buttons[NUMBER_OF_BUTTONS];
  init_button_geo(buttons);
  print_buttons(buttons);

  /* init_window(); */

  /* while (!WindowShouldClose()) { */
  /*   bool isFocused = IsWindowFocused(); */
  /*   draw_button_grid(&buttons); */

  /*   if (isFocused && !wasFocused) { */
  /*     printf("Window focused\n"); */
  /*   } else if (!isFocused && wasFocused) { */
  /*     printf("Window unfocused\n"); */
  /*   } */

  /*   BeginDrawing(); */
  /*   ClearBackground(RAYWHITE); */

  /*   if (isFocused) { */
  /*     DrawText("Hello, World!", 10, 10, 20, DARKGRAY); */
      
  /*   } else { */
  /*     sleep_program(20); */
  /*   } */

  /*   EndDrawing(); */
  /*   wasFocused = isFocused; */
  /* } */

  /* CloseWindow(); */

  return 0;
}
