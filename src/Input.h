#pragma once
#include <GLFW/glfw3.h>

class Input {
public:
    static void Init(GLFWwindow* window);
    static void Update();

    static bool IsKeyHeld(int key) { return keysHeld[key]; }
    static bool IsKeyPressed(int key) { return keysPressed[key]; }
    static bool IsKeyReleased(int key) { return keysReleased[key]; }

    static bool IsMousePressed() { return mousePressed; }
    static double GetMouseX() { return mouseX; }
    static double GetMouseY() { return mouseY; }
    static double GetMouseDeltaX() { return mouseX - lastMouseX; }
    static double GetMouseDeltaY() { return mouseY - lastMouseY; }

    static double GetScrollY() { return scrollDeltaY; }
    static bool IsMouseScrolled() { return scrollDeltaY != 0.0; }
private:
    static bool keysHeld[1024];           // Currently held
    static bool prevKeysHeld[1024];       // Last frame
    static bool keysPressed[1024];
    static bool keysReleased[1024];

    static bool mousePressed;
    static double mouseX, mouseY;
    static double lastMouseX, lastMouseY;

    static double scrollY, scrollDeltaY;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};