#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define APP_NAME "Simple Calculator"
#define NUMBER_OF_BUTTONS 18

static bool wasFocused = true;

typedef char **InputBuffer[256];
typedef struct {
  Rectangle rect;
  char tkn;
  Color color;
  bool is_square;
} Button;
struct AppState {
  InputBuffer in_buf;
  int result;
};

#define WIN_WIDTH 208
#define WIN_HEIGHT 327
#define FPS_TARGET 15
#define ROW_HEIGHT 50
#define COL_WIDTH 50
#define EDGE_PADDING 4

// Calculate the start positions (x, y) for the first row at the bottom
int startX = EDGE_PADDING;  
int startY = 327 - 2 - 49;  // Bottom edge padding and one button height
/* int topOffset = 88 + 4;  // Height of top areas + edge padding */

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

void DrawSquare(int posX, int posY, int size, Color color, char text) {
  DrawRectangle(posX, posY, size, size, color);
  char str[2] = {text, '\0'};
  DrawText(str, posX + 20, posY + 17, 20, DARKGRAY);
}

void DrawDSquare(int posX, int posY, int size, Color color, char text) {
  DrawRectangle(posX, posY, (size * 2) + 1, size, color);
  char str[2] = {text, '\0'};
  DrawText(str, posX + 42, posY + 17, 20, DARKGRAY);
}

void init_window() {
  // init window
  InitWindow(WIN_WIDTH, WIN_HEIGHT, APP_NAME);
  SetTargetFPS(FPS_TARGET);
}

void draw_button_grid(Button *btns) {
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
      Button btn = btns[i];
      Rectangle btn_rect = btn.rect;
      if (btn.is_square) {
        Rectangle btn_rect = btn.rect;
        DrawSquare(btn_rect.x, btn_rect.y, btn_rect.width, btn.color, btn.tkn);
      } else {
        DrawDSquare(btn_rect.x, btn_rect.y, btn_rect.width, btn.color, btn.tkn);
      }
    }
}

char find_collision(Vector2 click, Button *btns) {
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    Button btn = btns[i];
    if (CheckCollisionPointRec(click, btn.rect)) {
      return btn.tkn;
    }
  }
  return '\0';
}

static void print_buttons(Button btns[NUMBER_OF_BUTTONS]) {
  size_t n = 0;
  while (n < NUMBER_OF_BUTTONS) {
    printf("Button token: %c\n", btns[n].tkn);
    n++;
  }
}

static Button
make_btn(int x, int y, int sz, Color c, char tkn, bool is_square)
{
  Rectangle r = {.x = (float) x, .y = (float) y,
                 .width = (float) sz, .height = (float) sz};
  return (Button) {.rect = r, .color = c, .tkn = tkn, .is_square = is_square};
}

void init_button_geo(Button btns[NUMBER_OF_BUTTONS]) {
    
    btns[0] = make_btn(startX, startY, 49, LIGHTGRAY, 'C', false);
    btns[1] = make_btn(startX + (COL_WIDTH * 2), startY, 49, LIGHTGRAY, '=',
                       false);
    btns[2] = make_btn(startX + (COL_WIDTH * 3), startY - ROW_HEIGHT,
             49, LIGHTGRAY, '/', true);
    btns[3] = make_btn(startX + (COL_WIDTH * 3), startY - 2 * ROW_HEIGHT,
             49, LIGHTGRAY, '*', true);
    btns[4] = make_btn(startX + (COL_WIDTH * 3), startY - 3 * ROW_HEIGHT,
             49, LIGHTGRAY, '-', true);
    btns[5] = make_btn(startX + (COL_WIDTH * 3), startY - 4 * ROW_HEIGHT,
             49, LIGHTGRAY, '+', true);
    btns[6] = make_btn(startX, startY - ROW_HEIGHT,
             49, LIGHTGRAY, '(', true);
    btns[7] = make_btn(startX + (COL_WIDTH * 2), startY - ROW_HEIGHT,
             49, LIGHTGRAY, ')', true);
    btns[8] = make_btn(startX + COL_WIDTH, startY - ROW_HEIGHT,
             49, LIGHTGRAY, '0', true);
    btns[9] = make_btn(startX, startY - 2 * ROW_HEIGHT,
             49, LIGHTGRAY, '1', true);
    btns[10] = make_btn(startX + COL_WIDTH, startY - 2 * ROW_HEIGHT,
             49, LIGHTGRAY, '2', true);
    btns[11] = make_btn(startX + 2 * COL_WIDTH, startY - 2 * ROW_HEIGHT,
             49, LIGHTGRAY, '3', true);
    btns[12] = make_btn(startX, startY - 3 * ROW_HEIGHT,
             49, LIGHTGRAY, '4', true);
    btns[13] = make_btn(startX + COL_WIDTH, startY - 3 * ROW_HEIGHT,
             49, LIGHTGRAY, '5', true);
    btns[14] = make_btn(startX + 2 * COL_WIDTH, startY - 3 * ROW_HEIGHT,
             49, LIGHTGRAY, '6', true);
    btns[15] = make_btn(startX, startY - 4 * ROW_HEIGHT,
             49, LIGHTGRAY, '7', true);
    btns[16] = make_btn(startX + COL_WIDTH, startY - 4 * ROW_HEIGHT,
             49, LIGHTGRAY, '8', true);
    btns[17] = make_btn(startX + (COL_WIDTH * 2), startY - 4 * ROW_HEIGHT,
             49, LIGHTGRAY, '9', true);
}

/* NOTE: we calculated this, this is the lower bound for number of
 16pixel (@font size 20) letters that fit on 200px window */

#define MAX_EXPR_LENGTH 18

int main(void) {
  int fontSize = 18;
  Button buttons[NUMBER_OF_BUTTONS];
  init_button_geo(buttons);
  print_buttons(buttons);
  
  char expr[MAX_EXPR_LENGTH + 1];
  int expr_len = 0;
  expr[expr_len] = '0';

  init_window();
  // init_button_geo

  while (!WindowShouldClose()) {
    bool isFocused = IsWindowFocused();
    draw_button_grid(buttons);

    if (isFocused && !wasFocused) {
      printf("Window focused\n");
    } else if (!isFocused && wasFocused) {
      printf("Window unfocused\n");
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(expr, 190, 10, fontSize, DARKGRAY);
    // if (expr_len == 0) {
    // }

    if (isFocused) {
      // todo: when calculating result, change font of input to lighter color
      // todo: when calculating result, change font of result to darker color (darkgrey)

      // get mouse down first, then run collision logic
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        char tkn = find_collision(mousePos, buttons);
        if (tkn != '\0') {
          printf("found collision: %c\n", tkn);
          if (expr_len >= MAX_EXPR_LENGTH) {
            printf("hit max expr len\n");
            expr[expr_len] = '\0';
            continue;
          }

          expr[expr_len] = tkn;
          expr_len++;
        } else {
          printf("No collision, NULL\n");
        }
      
      }
    } else {
      sleep_program(20);
    }

    EndDrawing();
    wasFocused = isFocused;
  }

  CloseWindow();

  return 0;
}
