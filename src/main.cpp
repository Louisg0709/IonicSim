#include <fmt/core.h>
#include <raylib.h>

#include "main.h"

int main(){
    fmt::print("Starting app!\n");
    InitWindow(500, 500, "RaylibTemplate");

    SetTargetFPS(3);

    float LoopCount = 0;
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(ColorFromHSV(sinf(LoopCount),tanf(LoopCount),cosf(LoopCount)));
        EndDrawing();
        LoopCount++;
    }
}