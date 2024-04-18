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

Vector2 DrawSquare(int posX, int posY, int size, Color color, char *text) {
  DrawRectangle(posX, posY, size, size, color);
  DrawText(text, posX + 20, posY + 17, 20, DARKGRAY);
  return (Vector2){posX, posY};
}

Vector2 DrawDSquare(int posX, int posY, int size, Color color, char *text) {
  DrawRectangle(posX, posY, size * 2, size, color);
  DrawText(text, posX + 40, posY + 17, 20, DARKGRAY);
  return (Vector2){posX, posY};
}

void init_window() {
  // init window
  InitWindow(208, 330, APP_NAME);
  SetTargetFPS(15);
}

void draw_button_grid() {
    int topOffset = 88 + 4;  // Height of top areas + edge padding

    // Row and column setup
    int rowHeight = 50;
    int colWidth = 50;
    
    // Calculate the start positions (x, y) for the first row at the bottom
    int startX = 4;  // Edge padding
    int startY = 330 - 4 - 49;  // Bottom edge padding and one button height
    
    // Operator buttons on the right
    Vector2 divide_button = DrawSquare(startX + (colWidth * 3), startY - rowHeight, 49, LIGHTGRAY, "/");
    Vector2 multiply_button = DrawSquare(startX + (colWidth * 3), startY - 2 * rowHeight, 49, LIGHTGRAY, "*");
    Vector2 minus_button = DrawSquare(startX + (colWidth * 3), startY - 3 * rowHeight, 49, LIGHTGRAY, "-");
    Vector2 plus_button = DrawSquare(startX + (colWidth * 3), startY - 4 * rowHeight, 49, LIGHTGRAY, "+");
    
    // Special function buttons
    Vector2 clear_button = DrawDSquare(startX, startY, 49, LIGHTGRAY, "C");
    Vector2 equals_button = DrawDSquare(startX + (colWidth * 2), startY, 49, LIGHTGRAY, "=");
    Vector2 open_paren_button = DrawSquare(startX, startY - rowHeight, 49, LIGHTGRAY, "(");
    Vector2 close_paren_button = DrawSquare(startX + (colWidth * 2), startY - rowHeight, 49, LIGHTGRAY, ")");

    // Number buttons
    Vector2 zero_button = DrawSquare(startX + colWidth, startY - rowHeight, 49, LIGHTGRAY, "0");
    Vector2 one_button = DrawSquare(startX, startY - 2 * rowHeight, 49, LIGHTGRAY, "1");
    Vector2 two_button = DrawSquare(startX + colWidth, startY - 2 * rowHeight, 49, LIGHTGRAY, "2");
    Vector2 three_button = DrawSquare(startX + (2 * colWidth), startY - 2 * rowHeight, 49, LIGHTGRAY, "3");
    Vector2 four_button = DrawSquare(startX, startY - 3 * rowHeight, 49, LIGHTGRAY, "4");
    Vector2 five_button = DrawSquare(startX + colWidth, startY - 3 * rowHeight, 49, LIGHTGRAY, "5");
    Vector2 six_button = DrawSquare(startX + (2 * colWidth), startY - 3 * rowHeight, 49, LIGHTGRAY, "6");
    Vector2 seven_button = DrawSquare(startX, startY - 4 * rowHeight, 49, LIGHTGRAY, "7");
    Vector2 eight_button = DrawSquare(startX + colWidth, startY - 4 * rowHeight, 49, LIGHTGRAY, "8");
    Vector2 nine_button = DrawSquare(startX + (colWidth * 2), startY - 4 * rowHeight, 49, LIGHTGRAY, "9");
}


int main(void) {
  // const char *py_cmd = "python ./hello.py";

  // FILE *py_pipe = popen(py_cmd, "r");

  // int py_fd = fileno(py_pipe);
  // printf("FD: %d\n", py_fd);

  // char buf[256];
  // read(py_fd, buf, sizeof(buf));

  // printf("Data: %s\n", buf);

  // init window
  init_window();

  while (!WindowShouldClose()) {
    bool isFocused = IsWindowFocused();
    draw_button_grid();

    if (isFocused && !wasFocused) {
      printf("Window focused\n");
    } else if (!isFocused && wasFocused) {
      printf("Window unfocused\n");
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (isFocused) {
      DrawText("Hello, World!", 10, 10, 20, DARKGRAY);
      
    } else {
      sleep_program(20);
    }

    EndDrawing();
    wasFocused = isFocused;
  }

  CloseWindow();

  return 0;
}
