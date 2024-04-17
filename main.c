#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define APP_NAME "Simple Calculator"

static bool wasFocused = true;

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

void init_window() {
  // init window
  InitWindow(300, 700, APP_NAME);
  SetTargetFPS(15);
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
