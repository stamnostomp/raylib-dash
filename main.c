#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void DrawGauge(float value, float maxValue, Vector2 position, float radius, Color color, const char* label) {
    // Draw the outer circle
    DrawCircleV(position, radius, LIGHTGRAY);

    // Calculate the angle for the needle
    float angle = (value / maxValue) * 180 - 90; // Scale to 180 degrees
    Vector2 needleEnd = {
        position.x + radius * cosf(angle * (PI / 180)),
        position.y + radius * sinf(angle * (PI / 180))
    };

    // Draw the needle
    DrawLineV(position, needleEnd, color);

    // Draw the label
    DrawText(label, position.x - MeasureText(label, 20) / 2, position.y + radius + 10, 20, WHITE);
}

int main(void) {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rally Gauge Cluster");
    SetTargetFPS(60);

    float speed = 0.0f; // Speed value
    float rpm = 0.0f;   // RPM value
    float fuel = 0.0f;  // Fuel value

    while (!WindowShouldClose()) {
        // Update values (for demo purposes)
        speed += 1.0f; // Increment speed
        if (speed > 200) speed = 0; // Loop speed

        rpm += 10.0f; // Increment RPM
        if (rpm > 8000) rpm = 0; // Loop RPM

        fuel -= 0.1f; // Decrement fuel
        if (fuel < 0) fuel = 100; // Reset fuel

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw gauges
        DrawGauge(speed, 200, (Vector2){ 200, 300 }, 100, RED, "Speed");
        DrawGauge(rpm, 8000, (Vector2){ 600, 300 }, 100, GREEN, "RPM");
        DrawGauge(fuel, 100, (Vector2){ 400, 500 }, 100, BLUE, "Fuel");

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}
