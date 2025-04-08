#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputManager
{
private:
    // Constants
    constexpr static int _MAX_RESSETABLE_KEY_COUNT = 20;
    constexpr static int _MAX_KEY_COUNT = 347; // 346 max keycode
    constexpr static int _MAX_MOUSE_BUTTON_COUNT = 5;

    GLFWwindow* refWindow;
public:

    // Structs
    typedef struct {
        glm::vec2 MousePosition;
        glm::vec2 MouseDeltaPosition;
        glm::vec2 MouseScrollOffset;
        char MouseButton[_MAX_MOUSE_BUTTON_COUNT];
        char MouseButtonDown[_MAX_MOUSE_BUTTON_COUNT];
        char MouseButtonUp[_MAX_MOUSE_BUTTON_COUNT];
        GLenum Status;
    }MouseInputs;
    typedef struct {
        int Key[_MAX_KEY_COUNT];
        char KeyDown[_MAX_KEY_COUNT];
        char KeyUp[_MAX_KEY_COUNT];
    }KeyboardInputs;

	class HWInputs
	{
    public:
        // Variables
        static MouseInputs Mouse;
        static KeyboardInputs Keyboard;

        // Functions
        static void Update();
        static void Initialise(GLFWwindow* window/*, Window* windowData*/);
        static void SetMouseStatus(GLFWwindow* window, GLenum status); // GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_NORMAL
    private:
        // Variables
        static int RESETTABLE_KEYS[_MAX_RESSETABLE_KEY_COUNT];
        static int RESETTABLE_KEY_COUNT;
        static glm::vec2 LAST_MOUSE_POSITION;
        static char TEST_MODE;
        static char ReceivedMousePositionUpdate;
        static char ReceivedMouseScrollUpdate;

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	};

	bool IsSpriteClicked();
	bool IsSpriteHovered();

    void Update();
    void Initialise(GLFWwindow* window);
    void SetMouseStatus(int status);

    MouseInputs IMouse();
    bool Button(int button);
    bool ButtonDown(int button);
    bool ButtonUp(int button);
    glm::vec2 MousePosition();
    glm::vec2 MouseDeltaPosition();
    glm::vec2 MouseScrollOffset();
    int MouseStatus();

    KeyboardInputs IKeyboard();
    bool Key(int key);
    bool KeyDown(int key);
    bool KeyUp(int key);


//	sf::Vector2i GetMousePosition();
};
