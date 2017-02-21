#ifndef APP_HPP
#define APP_HPP

struct GLFWwindow;
#include "rendering/renderer_2d.hpp"
#include <memory>
#include "rendering/postprocessor.hpp"
#include "rendering/font_loader.hpp"

class App
{
public:
    App();
    ~App();
    App(const App&) = delete;

private:
    GLFWwindow* window;
    std::unique_ptr<Renderer_2D> renderer_2D;
    std::unique_ptr<Postprocessor> pp_unit;
    Font_loader font_loader;

    void run();
    void processInput();
    void update(float dt);
    void render();
    void set_opengl_states();

    static bool isCurrent;

    static void error_callback(int error, const char* description);
};

#endif // APP_HPP
