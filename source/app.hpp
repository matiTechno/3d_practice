#ifndef APP_HPP
#define APP_HPP

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "rendering/renderer_2d.hpp"
#include <memory>
#include "rendering/postprocessor.hpp"
#include "rendering/font_loader.hpp"
#include "rendering/renderer_3d.hpp"
// for testing
#include "rendering/model_3d.hpp"
#include "rendering/camera_3d.hpp"

class Del_glfw
{
public:
    Del_glfw() = default;
    ~Del_glfw()
    {
        glfwTerminate();
    }
    Del_glfw(const Del_glfw&) = delete;
    Del_glfw& operator=(const Del_glfw&) = delete;
};

class App
{
public:
    App();
    App(const App&) = delete;

private:
    Del_glfw del_glfw;
    GLFWwindow* window;
    std::unique_ptr<Renderer_2D> renderer_2D;
    std::unique_ptr<Postprocessor> pp_unit;
    Font_loader font_loader;
    std::unique_ptr<Renderer_3D> renderer_3D;
    Camera_3D camera;

    void run();
    void processInput();
    void update(float dt);
    void render();
    void set_opengl_states();

    static bool isCurrent;

    static void error_callback(int error, const char* description);

    // for testing
    std::unique_ptr<Font> font;
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Model_3D> model;
    float frametime_panel;
};

#endif // APP_HPP
