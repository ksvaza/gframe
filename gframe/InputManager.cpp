#include "InputManager.hpp"
#include <stdio.h>

// Define static member variables
InputManager::MouseInputs InputManager::HWInputs::Mouse;
InputManager::KeyboardInputs InputManager::HWInputs::Keyboard;
int InputManager::HWInputs::RESETTABLE_KEYS[_MAX_RESSETABLE_KEY_COUNT];
int InputManager::HWInputs::RESETTABLE_KEY_COUNT = 0;
glm::vec2 InputManager::HWInputs::LAST_MOUSE_POSITION(0.0f, 0.0f);
char InputManager::HWInputs::TEST_MODE = 0;
char InputManager::HWInputs::ReceivedMousePositionUpdate = 0;/*
Window* HWInputs::WindowData = NULL;*/

// Functions
//void InputManager::HWInputs::SetTestMode(char mode)
//{
//    TEST_MODE = mode;
//}
void InputManager::HWInputs::Update()
{
    if (RESETTABLE_KEY_COUNT > 0)
    {
        for (int i = 0; i < RESETTABLE_KEY_COUNT; i++)
        {
            Keyboard.KeyDown[RESETTABLE_KEYS[i]] = 0;
            Keyboard.KeyUp[RESETTABLE_KEYS[i]] = 0;
        }
        RESETTABLE_KEY_COUNT = 0;
    }
    for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++)
    {
        Mouse.MouseButtonDown[i] = 0;
        Mouse.MouseButtonUp[i] = 0;
    }
    if (ReceivedMousePositionUpdate)
    {
        Mouse.MouseDeltaPosition = Mouse.MousePosition - LAST_MOUSE_POSITION;
        LAST_MOUSE_POSITION = Mouse.MousePosition;
        ReceivedMousePositionUpdate = 0;
    }
    else
    {
        Mouse.MouseDeltaPosition = glm::vec2(0.0f, 0.0f);
    }
}
void InputManager::HWInputs::Initialise(GLFWwindow* window/*, Window* windowData*/)
{
    // Set all keys to 0
    for (int i = 0; i < _MAX_KEY_COUNT; i++)
    {
        Keyboard.Key[i] = 0;
        Keyboard.KeyDown[i] = 0;
        Keyboard.KeyUp[i] = 0;
    }
    // Set all mouse buttons to 0
    for (int i = 0; i < _MAX_MOUSE_BUTTON_COUNT; i++)
    {
        Mouse.MouseButton[i] = 0;
        Mouse.MouseButtonDown[i] = 0;
        Mouse.MouseButtonUp[i] = 0;
    }
    // Set all resettable keys to 0
    for (int i = 0; i < _MAX_RESSETABLE_KEY_COUNT; i++)
    {
        RESETTABLE_KEYS[i] = 0;
    }
    // Set last mouse position to 0
    LAST_MOUSE_POSITION = glm::vec2(0.0f, 0.0f);

    // Set callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // sussy
    //glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

    // vajag
    if (glfwRawMouseMotionSupported()) { glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); }

    //WindowData = windowData;
}
void InputManager::HWInputs::SetMouseStatus(GLFWwindow* window, GLenum status)
{
    glfwSetInputMode(window, GLFW_CURSOR, status);
    Mouse.Status = status;
}

// Callbacks
void InputManager::HWInputs::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key >= _MAX_KEY_COUNT) { return; }
    Keyboard.Key[key] = action;
    if (action == GLFW_PRESS) { Keyboard.KeyDown[key] = 1; }
    else if (!action) { Keyboard.KeyUp[key] = 1; }
    RESETTABLE_KEYS[RESETTABLE_KEY_COUNT] = key;
    RESETTABLE_KEY_COUNT++;
    if (TEST_MODE)
    {
        if (action) { printf("Key: %d : %c pressed\n", key, key); }
        else { printf("Key: %d : %c released\n", key, key); }
    }
    //if (Keyboard.KeyDown[key]) { printf("%c", key); }
    if (RESETTABLE_KEY_COUNT >= _MAX_RESSETABLE_KEY_COUNT) { printf("\n_MAX_RESSETABLE_KEY_COUNT of %d exceded\n\n", _MAX_RESSETABLE_KEY_COUNT); RESETTABLE_KEY_COUNT--; }
}
void InputManager::HWInputs::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Mouse.MousePosition = glm::vec2((float)xpos, (float)ypos);
    ReceivedMousePositionUpdate = 1;
}
void InputManager::HWInputs::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button >= _MAX_MOUSE_BUTTON_COUNT) { return; }
    Mouse.MouseButton[button] = action;
    if (action)
    {
        Mouse.MouseButtonDown[button] = 1;
        if (TEST_MODE) { printf("Mouse button: %d pressed\n", button); }
    }
    else
    {
        Mouse.MouseButtonUp[button] = 1;
        if (TEST_MODE) { printf("Mouse button: %d released\n", button); }
    }
}
void InputManager::HWInputs::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Mouse.MouseScrollOffset = glm::vec2((float)xoffset, (float)yoffset);
    if (TEST_MODE)
    {
        if (xoffset < 0) { printf("Mouse scrolled right\n"); }
        else if (xoffset > 0) { printf("Mouse scrolled left\n"); }
        if (yoffset > 0) { printf("Mouse scrolled up\n"); }
        else if (yoffset < 0) { printf("Mouse scrolled down\n"); }
    }
}