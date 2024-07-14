#include <raylib.h>

int main(void) {

  InitWindow(800, 600, "Hello World");
  
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    EndDrawing();
  }
  
  CloseWindow();
  
  return 0;
}
