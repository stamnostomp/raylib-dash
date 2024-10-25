#include "raylib.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define MAX_RPM 8000.0f
#define BUFFER 40 // Buffer space for the RPM bar

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Digital Dash Cluster");
    SetTargetFPS(60);

    float speed = 0.0f;          // Speed in km/h
    float rpm = 0.0f;            // RPM
    float fuel = 100.0f;         // Fuel in percentage
    float oilPressure = 50.0f;   // Oil pressure in PSI
    float coolantTemp = 70.0f;   // Coolant temperature in °C
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    while (!WindowShouldClose()) // Main game loop
    {
        // Update values (for simulation purposes)
        speed += 0.5f;
        if (speed > 220.0f) speed = 0.0f; // Reset speed
        rpm += 10.0f;
        if (rpm > MAX_RPM) rpm = 0.0f; // Reset RPM
        fuel -= 0.1f;
        if (fuel < 0.0f) fuel = 0.0f; // Minimum fuel
        oilPressure += 0.001f;
        if (oilPressure > 80.0f) oilPressure = 30.0f; // Reset oil pressure
        coolantTemp += 0.2f;
        if (coolantTemp > 100.0f) coolantTemp = 60.0f; // Reset coolant temp

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw speed (centered)
        //DrawRectangle(440,300,400,200,DARKGRAY);
        int speedSize = 250;
        const char *speedText = TextFormat("%.0f", speed);
        int speedTextWidth = MeasureText(speedText, speedSize);

        DrawText(speedText, centerX - (speedTextWidth /  2), centerY, speedSize, GREEN);
        // Draw RPM
        DrawText("RPM", 80, 220, 30, BLACK); // Moved down to 220
        DrawText(TextFormat("%.1f", rpm), 80, 260, 50, DARKGRAY); // Moved down to 260

        // Draw fuel
        DrawText("Fuel", 80, 340, 30, BLACK); // Moved down to 340
        DrawText(TextFormat("%.1f%%", fuel), 80, 380, 50, DARKGRAY); // Moved down to 380

        // Draw Oil Pressure
        DrawText("Oil Pressure", 1020, 340, 30, BLACK); // Moved down to 340
        DrawText(TextFormat("%.1f PSI", oilPressure), 1020, 380, 50, DARKGRAY); // Moved down to 380

        // Draw Coolant temp
        DrawText("Coolant Temp", 1020, 220, 30, BLACK); // Moved down to 340
        DrawText(TextFormat("%.1f °C", coolantTemp), 1020, 260, 50, DARKGRAY); // Moved down to 380



        // Draw RPM gauge background with buffer
        DrawRectangle(BUFFER, 60, SCREEN_WIDTH - 2 * BUFFER, 120, LIGHTGRAY); // Added buffer

        // Determine RPM color based on stage
        Color rpmColor;
        if (rpm < 5000.0f) {
            rpmColor = GREEN; // Safe zone
        } else if (rpm < 6500.0f) {
            rpmColor = YELLOW; // Caution zone
        } else if (rpm < 7500.0f) {
            rpmColor = ORANGE; // Danger zone
        } else {
            rpmColor = RED; // Red at max RPM
        }

        // Draw RPM filled bar
        float rpmBarWidth = ((rpm / MAX_RPM) * (SCREEN_WIDTH - 2 * BUFFER));
        DrawRectangle(BUFFER, 60, rpmBarWidth, 120, rpmColor); // Full width with buffer

        // Draw RPM indicators below the bar
        for (int i = 0; i <= 8; i++) {
            float indicatorX = BUFFER + (i * ((SCREEN_WIDTH - 2 * BUFFER) / 8));
            DrawText(TextFormat("%d", i * 1000), indicatorX - 15, 180, 20, BLACK); // Adjusted Y-coordinate to 180
        }

        // Draw oil pressure and coolant temp on the left side
        // DrawText("Oil Pressure:", 80, 430, 25, BLACK); // Left side
        //DrawText(TextFormat("%.1f PSI", oilPressure), 80, 460, 25, DARKGRAY); // Same style
        //DrawText("Coolant Temp:", 80, 500, 25, BLACK); // Left side
        //DrawText(TextFormat("%.1f °C", coolantTemp), 80, 530, 25, DARKGRAY); // Same style

        // Draw larger "SHIFT!" message box at 95% RPM
        if (rpm >= 0.90f * MAX_RPM) {
            int boxW = 600;
            int boxH = 200;
            const char *shiftText= "SHIFT!";
            int shiftTextWidth = MeasureText(shiftText, 70);
            DrawRectangle(centerX - (boxW / 2), centerY - (boxH / 2) + 35 , boxW, boxH, RED); // Larger box
            DrawText("SHIFT!", centerX - (shiftTextWidth / 2) , centerY, 70, WHITE); // Centered text
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
