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
  DrawText(text, posX + 20, posY + 34, 20, DARKGRAY);
  return (Vector2){posX, posY};
}

void init_window() {
  // init window
  InitWindow(205, 325, APP_NAME);
  SetTargetFPS(15);
}

void draw_button_grid() {
    Vector2 divide_button = DrawSquare(150, 50, 49, LIGHTGRAY, "/");
    Vector2 multiply_button = DrawSquare(150, 100, 49, LIGHTGRAY, "*");
    Vector2 minus_button = DrawSquare(150, 150, 49, LIGHTGRAY, "-");
    Vector2 plus_button = DrawSquare(150, 200, 49, LIGHTGRAY, "+");
    Vector2 parens_open_button = DrawSquare(2, 150, 49, LIGHTGRAY, "(");
    Vector2 parens_close_button = DrawSquare(100, 450, 49, LIGHTGRAY, ")");
    Vector2 clear_button = DrawDSquare(2, 250, 49, LIGHTGRAY, "C");
    Vector2 equals_button = DrawDSquare(100, 250, 49, LIGHTGRAY, "=");

    Vector2 zero_button = DrawSquare(50, 200, 49, LIGHTGRAY, "0");
    Vector2 one_button = DrawSquare(2, 150, 49, LIGHTGRAY, "1");
    Vector2 two_button = DrawSquare(50, 150, 49, LIGHTGRAY, "2");
    Vector2 three_button = DrawSquare(100, 150, 49, LIGHTGRAY, "3");
    Vector2 four_button = DrawSquare(2, 100, 49, LIGHTGRAY, "4");
    Vector2 five_button = DrawSquare(50, 100, 49, LIGHTGRAY, "5");
    Vector2 six_button = DrawSquare(100, 100, 49, LIGHTGRAY, "6");
    Vector2 seven_button = DrawSquare(2, 50, 49, LIGHTGRAY, "7");
    Vector2 eight_button = DrawSquare(50, 50, 49, LIGHTGRAY, "8");
    Vector2 nine_button = DrawSquare(100, 50, 49, LIGHTGRAY, "9");
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
