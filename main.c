#include "raylib.h"
#include "pthread.h"
#include <stdio.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define MAX_RPM 8000.0f
#define BUFFER 40 // Buffer space for the RPM bar
#define BACKGROUND (Color){20,27,30,255}
#define RED (Color){229, 116,166,255}
#define GREEN (Color){140,207,126,255}
#define YELLOW (Color){229, 199,107,255}
#define MAGENTA (Color){196, 127, 213, 255}
#define CYAN (Color){108, 191, 191, 255}
#define LIGHTGRAY (Color){179, 185, 184, 255}
#define WHITE (Color){255, 255, 255, 255}

// Dimensions for the signal indicator grid
#define GRID_WIDTH 4
#define GRID_HEIGHT 2
#define BOX_HIGHT 100
#define BOX_WIDTH 160
#define BOX_MARGIN 10

void drawWarnings(void) {
    DrawRectangle(1000, 80, 200, 100, CYAN);
}

void drawSignalIndicatorGrid(void) {
    Color boxColour = LIGHTGRAY;
    for (int row = 0; row < GRID_HEIGHT; row++) {
        for (int col = 0; col < GRID_WIDTH; col++) {
            int x = (SCREEN_WIDTH / 2 - (GRID_WIDTH * (BOX_WIDTH + BOX_MARGIN)) / 2) + col * (BOX_WIDTH + BOX_MARGIN);
            int y = SCREEN_HEIGHT - (GRID_HEIGHT * (BOX_HIGHT + BOX_MARGIN)) - 40 + row * (BOX_HIGHT + BOX_MARGIN);

            if(row == 0 && col == 1) { // leftIndicator
               boxColour = GREEN;
            } else if (row == 0 && col == 2) { // right indicator
               boxColour = GREEN;
            } else {
                boxColour = LIGHTGRAY;
            }

            DrawRectangle(x, y, BOX_WIDTH , BOX_HIGHT, boxColour);
            DrawRectangleLines(x, y, BOX_WIDTH, BOX_HIGHT, WHITE); // Draw box outline
        }
    }
}

int main(void) {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Digital Dash Cluster");
    SetTargetFPS(60);

    float speed = 0.0f;          // Speed in km/h
    float rpm = 0.0f;            // RPM
    float fuel = 100.0f;         // Fuel in percentage
    float oilPressure = 50.0f;   // Oil pressure in PSI
    float coolantTemp = 70.0f;   // Coolant temperature in °C
    float fulePressure = 70.0f;  // Fuel pressure
    float volts = 12.5f;         // Voltage
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    // Flashing control
    float flashDuration = 0.5f;
    float timer = 0.0f;
    bool showText = true;
    bool testing = true;

    while (!WindowShouldClose()) { // Main game loop
        if (testing) {
            // Simulate data updates
            speed += 0.5f;
            if (speed > 220.0f) speed = 0.0f; // Reset speed
            rpm += 10.0f;
            if (rpm > MAX_RPM) rpm = 0.0f; // Reset RPM
            fuel -= 0.1f;
            if (fuel < 0.0f) fuel = 100.0f; // Minimum fuel
            oilPressure += 0.001f;
            if (oilPressure > 80.0f) oilPressure = 30.0f; // Reset oil pressure
            coolantTemp += 0.2f;
            if (coolantTemp > 100.0f) coolantTemp = 60.0f; // Reset coolant temp
            fulePressure += 0.2f;
            if (fulePressure > 90.0f) fulePressure = 70.0f;
            volts += 0.002f;
            if (volts > 14.5f) volts = 10.5f;
        }

        // Update flashing text logic
        timer += GetFrameTime();
        if (timer >= flashDuration) {
            showText = !showText;
            timer = 0.0f;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BACKGROUND);

        // Draw the main elements
        drawWarnings();

        // Draw speed
        int speedSize = 250;
        const char *speedText = TextFormat("%.0f", speed);
        int speedTextWidth = MeasureText(speedText, speedSize);
        DrawText(speedText, centerX - (speedTextWidth / 2), centerY - 150, speedSize, CYAN);

        // Draw RPM
        DrawText("RPM", 80, 220, 30, MAGENTA);
        DrawText(TextFormat("%.1f", rpm), 80, 260, 50, (rpm < 6800.0f) ? LIGHTGRAY : RED);

        // Draw Fuel
        DrawText("Fuel", 80, 340, 30, MAGENTA);
        DrawText(TextFormat("%.1f%%", fuel), 80, 380, 50, (fuel > 20.0f) ? LIGHTGRAY : RED);
        if (fuel <= 20.0f && showText) {
            DrawText("!", 220, 380, 60, RED);
        }

        // Draw Oil Pressure
        DrawText("Oil Pressure", 1020, 340, 30, MAGENTA);
        DrawText(TextFormat("%.1f PSI", oilPressure), 1020, 380, 50, LIGHTGRAY);

        // Draw Coolant Temperature
        DrawText("Coolant Temp", 1020, 220, 30, MAGENTA);
        DrawText(TextFormat("%.1f °C", coolantTemp), 1020, 260, 50, (coolantTemp < 90.0f) ? LIGHTGRAY : RED);
        if (coolantTemp >= 90.0f && showText) {
            DrawText("!", 1210, 260, 60, RED);
        }

        // Draw RPM Gauge Background
        DrawRectangle(BUFFER, 60, SCREEN_WIDTH - 2 * BUFFER, 120, LIGHTGRAY);

        // Draw RPM Bar
        Color rpmColor = (rpm < 5000.0f) ? GREEN : (rpm < 6500.0f) ? YELLOW : (rpm < 7100.0f) ? YELLOW : RED;
        float rpmBarWidth = ((rpm / MAX_RPM) * (SCREEN_WIDTH - 2 * BUFFER));
        DrawRectangle(BUFFER, 60, rpmBarWidth, 120, rpmColor);


        for (int i = 0; i <= 8; i++) {
            float indicatorX = BUFFER + (i * ((SCREEN_WIDTH - 2 * BUFFER) / 8));
            DrawText(TextFormat("%d", i * 100), indicatorX - 15, 190, 20, WHITE); // Adjusted Y-coordinate to 180
        }

         if (rpm >= 0.90f * MAX_RPM) {
            int boxW = 600;
            int boxH = 200;
            const char *shiftText= "SHIFT!";
            int shiftTextWidth = MeasureText(shiftText, 70);
            DrawRectangle(centerX - (boxW / 2), centerY - (boxH / 2) + 35 , boxW, boxH, RED); // Larger box
            DrawText("SHIFT!", centerX - (shiftTextWidth / 2) , centerY, 70, WHITE); // Centered text
        }


         // Draw Coolant temp
        if (coolantTemp < 90.0f){
            DrawText("Coolant Temp", 1020, 220, 30, MAGENTA);
            DrawText(TextFormat("%.1f °C", coolantTemp), 1020, 260, 50, LIGHTGRAY);
        } else {
            DrawText("Coolant Temp", 1020, 220, 30, MAGENTA);
            DrawText(TextFormat("%.1f °C", coolantTemp), 1020, 260, 50, RED);
            if (showText){
                DrawText("!", 1210, 260, 60, RED);
            }
        }

        // Draw Fuel Pressure
        DrawText("Fuel Pressure", 1020, 460,30, MAGENTA);
        DrawText(TextFormat("%.1f PSI", fulePressure), 1020, 500, 50, LIGHTGRAY);

        // Draw Battery volts
        if ( volts > 11.5f ){
            DrawText("Battery", 80, 460, 30, MAGENTA);
            DrawText(TextFormat("%.1f V", volts), 80, 500, 50, LIGHTGRAY);
        } else {
            DrawText("Battery", 80, 460, 30, MAGENTA);
            DrawText(TextFormat("%.1f V", volts), 80, 500, 50, RED);
            if (showText){
                DrawText("!", 240, 500, 60, RED);
            }
        }

        // Draw signal indicator grid
        drawSignalIndicatorGrid();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context
    return 0;
}
