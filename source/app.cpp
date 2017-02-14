#include "app.hpp"
#include <assert.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <string>

bool App::isCurrent  = false;

void App::error_callback(int, const char* description)
{
    std::cout << "glfw error: " << description << std::endl;
}

App::App()
{
    assert(!isCurrent);
    isCurrent = true;

    glfwSetErrorCallback(error_callback);

    std::cout << "glfw compile time info: " << glfwGetVersionString() << std::endl;

    {
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        std::string version = std::to_string(major) + '.' + std::to_string(minor) + '.' + std::to_string(revision);
        std::cout << "glfw runtime version: " + version << std::endl;
    }

    if(!glfwInit())
        throw std::runtime_error("glfw initialization failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, 0);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(800, 600, "3d_practice", nullptr, nullptr);

    if(!window)
        throw std::runtime_error("glfw window creation failed");

    auto monitor = glfwGetPrimaryMonitor();
    auto video_mode = glfwGetVideoMode(monitor);
    int win_width, win_height;
    glfwGetWindowSize(window, &win_width, &win_height);
    glfwSetWindowPos(window, video_mode->width / 2 - win_width / 2, video_mode->height / 2 - win_height / 2);

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        throw std::runtime_error("gladLoadGLLoader failed");

    std::cout << "vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    if(!GLAD_GL_ARB_texture_storage)
        throw std::runtime_error("ARB_texture_storage not available");

    glfwSwapInterval(1);

    run();
}

App::~App()
{
    glfwTerminate();
}

void App::run()
{
    auto current_time = std::chrono::high_resolution_clock::now();

    while(!glfwWindowShouldClose(window))
    {
        processInput();

        auto new_time = std::chrono::high_resolution_clock::now();
        auto frame_time = std::chrono::duration<float>(new_time - current_time).count();
        current_time = new_time;

        static float acc_time = 0.f;
        static int frame_count = 0;
        ++frame_count;
        acc_time += frame_time;

        if(acc_time > 1.f)
        {
            std::cout << "frametime: " << acc_time / static_cast<float>(frame_count) << '\r';
            std::cout.flush();
            acc_time = 0.f;
            frame_count = 0;
        }

        update(frame_time);

        render();
    }
}

void App::processInput()
{
    glfwPollEvents();
}

void App::update(float)
{

}

void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
}

void App::set_opengl_states()
{

}
