#ifndef APP_HPP
#define APP_HPP

struct GLFWwindow;

class App
{
public:
    App();
    ~App();
    App(const App&) = delete;
    App& operator=(const App&) = delete;

private:
    GLFWwindow* window;

    void run();
    void processInput();
    void update(float dt);
    void render();
    void set_opengl_states();

    static bool isCurrent;

    static void error_callback(int error, const char* description);
};

#endif // APP_HPP
