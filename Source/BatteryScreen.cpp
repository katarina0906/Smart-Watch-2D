#include "../Header/BatteryScreen.h"
#include "../Header/Renderer.h"
#include "../Header/ScreenManager.h"
#include <GLFW/glfw3.h>

float batteryPercent = 100.0f;
double batteryTimer = 0.0;

void BatteryScreen_Init()
{
    batteryPercent = 100.0f;
    batteryTimer = 0.0;
}

void BatteryScreen_Update(double dt)
{
    batteryTimer += dt;

    if (batteryTimer >= 10.0)
    {
        batteryTimer = 0.0;
        batteryPercent -= 1.0f;
        if (batteryPercent < 0.0f)
            batteryPercent = 0.0f;
    }

    if (batteryPercent <= 0.0f)
    {
        GLFWwindow* win = glfwGetCurrentContext();
        if (win) glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

void BatteryScreen_Render(unsigned int shader, unsigned int quadVAO)
{
    float cx = 0.0f;
    float cy = 0.00f;

    float scaleArrow = 0.0032f;
    Renderer_DrawText("<", cx - 0.63f, cy, scaleArrow, 1, 1, 1);

    char txt[16];
    sprintf(txt, "%.0f%%", batteryPercent);
    Renderer_DrawText(txt, cx - 0.05f, cy + 0.20f, 0.0048f, 1, 1, 1);

    float outerW = 0.45f;
    float outerH = 0.18f;

    drawQuadRect(shader, quadVAO,
        cx, cy,
        outerW, outerH,
        1, 1, 1, 1);

    float fillRatio = batteryPercent / 100.0f;
    float fillW = outerW * fillRatio;
    float fillH = outerH * 0.80f;

    float rightX = cx + (outerW / 2.0f);
    float leftX = rightX - fillW;
    float fillCenterX = (leftX + rightX) * 0.5f;

    float r = 0, g = 1, b = 0;
    if (batteryPercent <= 10) { r = 1; g = 0; b = 0; }
    else if (batteryPercent <= 20) { r = 1; g = 1; b = 0; }

    drawQuadRect(shader, quadVAO,
        fillCenterX,
        cy,
        fillW,
        fillH,
        r, g, b, 1);
}
