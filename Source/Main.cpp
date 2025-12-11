#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"
#include "../Header/Renderer.h"
#include "../Header/TimeScreen.h"
#include "../Header/ScreenManager.h"
#include "../Header/BPMScreen.h"
#include "../Header/BatteryScreen.h"

#include <chrono>
#include <thread>
#include <iostream>

static bool clickReleased = true;

unsigned int tex_nameplate = 0;

void GetMouseNDC(GLFWwindow* window, float& x, float& y)
{
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    x = (float)(mx / w * 2.0 - 1.0f);
    y = (float)(1.0f - my / h * 2.0f);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(
        mode->width,
        mode->height,
        "Pametni sat",
        monitor,       
        nullptr
    );

    /* 
    GLFWwindow* window = glfwCreateWindow(
        1000,
        1000,
        "Pametni sat",
        nullptr,
        nullptr
    );
    */

    if (!window) return endProgram("Prozor nije uspeo da se kreira.");

    glfwMakeContextCurrent(window);

    GLFWcursor* heartCursor = loadImageToCursor("Resources/Textures/hello.png");
    if (heartCursor != nullptr)
        glfwSetCursor(window, heartCursor);

    if (glewInit() != GLEW_OK)
        return endProgram("GLEW nije uspeo da se inicijalizuje.");

    float quadVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int shader = createShader(
        "Resources/Shaders/basic.vert",
        "Resources/Shaders/basic.frag"
    );

    Renderer_Init();
    TimeScreen_Init();
    BPMScreen_Init();
    BatteryScreen_Init();

    tex_nameplate = loadImageToTexture("Resources/Textures/nameplate2.png");
    glBindTexture(GL_TEXTURE_2D, tex_nameplate);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    currentScreen = SCREEN_TIME;

    glfwSwapInterval(0); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.18f, 0.10f, 0.12f, 1.0f);

    const double targetFrameTime = 1.0 / 75.0;
    double lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double frameStart = glfwGetTime();
        double deltaTime = frameStart - lastFrameTime;
        lastFrameTime = frameStart;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        float mx, my;
        GetMouseNDC(window, mx, my);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            clickReleased = true;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
            clickReleased)
        {
            clickReleased = false;

            if (currentScreen == SCREEN_TIME)
            {
                if (mx > 0.48f && mx < 0.60f &&
                    my > -0.10f && my < 0.10f)
                {
                    currentScreen = SCREEN_BPM;
                }
            }
            else if (currentScreen == SCREEN_BPM)
            {
                if (mx > -0.70f && mx < -0.55f &&
                    my > -0.12f && my < 0.12f)
                {
                    currentScreen = SCREEN_TIME;
                }
                else if (mx > 0.48f && mx < 0.60f &&
                    my > -0.12f && my < 0.12f)
                {
                    currentScreen = SCREEN_BATTERY;
                }
            }
            else if (currentScreen == SCREEN_BATTERY)
            {
                if (mx > -0.70f && mx < -0.55f &&
                    my > -0.12f && my < 0.12f)
                {
                    currentScreen = SCREEN_BPM;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        drawTexturedQuad(shader, quadVAO,
            0.0f, 0.0f,
            1.0f, 1.0f,
            tex_watchFrame);

        drawTexturedQuad(
            shader,
            quadVAO,
            0.80f, -0.90f,   
            0.3f, 0.3f,    
            tex_nameplate
        );

        TimeScreen_Update(deltaTime);

        bool running = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
        BPMScreen_Update(deltaTime, running);

        BatteryScreen_Update(deltaTime);

        if (currentScreen == SCREEN_TIME)
        {
            TimeScreen_Render(shader, quadVAO);
        }
        else if (currentScreen == SCREEN_BPM)
        {
            BPMScreen_Render(shader, quadVAO);
        }
        else if (currentScreen == SCREEN_BATTERY)
        {
            BatteryScreen_Render(shader, quadVAO);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        double frameEnd = glfwGetTime();
        double frameDuration = frameEnd - frameStart;

        if (frameDuration < targetFrameTime)
        {
            double sleepTime = targetFrameTime - frameDuration;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
