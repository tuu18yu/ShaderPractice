#include "input.h"

bool Input::keysHeld[1024] = { false };
bool Input::prevKeysHeld[1024] = { false };
bool Input::keysPressed[1024] = { false };
bool Input::keysReleased[1024] = { false };

bool Input::mousePressed = false;
double Input::mouseX = 0.0, Input::mouseY = 0.0;
double Input::lastMouseX = 0.0, Input::lastMouseY = 0.0;

double Input::scrollY = 0.0, Input::scrollDeltaY = 0.0;

void Input::Init(GLFWwindow* window) 
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, MouseScrollCallback);
}

void Input::Update() 
{
    for (int i = 0; i < 1024; ++i) 
    {
        keysPressed[i]  = (!prevKeysHeld[i] && keysHeld[i]);  // false → true
        keysReleased[i] = (prevKeysHeld[i] && !keysHeld[i]);  // true → false
        prevKeysHeld[i] = keysHeld[i];
    }

    lastMouseX = mouseX;
    lastMouseY = mouseY;
    scrollDeltaY = 0.0;
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keysHeld[key] = true;
        else if (action == GLFW_RELEASE) keysHeld[key] = false;
    }
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) { mousePressed = (action == GLFW_PRESS); }
}

void Input::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    mouseX = xpos;
    mouseY = ypos;
}

void Input::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    scrollDeltaY += yoffset;
}