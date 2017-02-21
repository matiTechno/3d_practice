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
    //glfwWindowHint(GLFW_SAMPLES, 8);
    //glfwWindowHint(GLFW_SRGB_CAPABLE, 1);

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

    renderer_2D = std::make_unique<Renderer_2D>();

    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        pp_unit = std::make_unique<Postprocessor>(width, height);
    }

    set_opengl_states();
    run();
}

App::~App()
{
    renderer_2D.reset();
    pp_unit.reset();
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

#include "rendering/sprite.hpp"
#include "rendering/text.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    //glViewport(0, 0, width, height);
    pp_unit->set_new_size(width, height);
    pp_unit->begRender();
    {
        glm::mat4 proj = glm::ortho(0.f, static_cast<float>(width),
                                    static_cast<float>(height), 0.f);
        renderer_2D->load_projection(proj);

        Texture tex("res/block1_Jerom.png");
        Sprite sprite;
        sprite.position = glm::vec2(100.f, 100.f);
        sprite.size = glm::vec2(100.f, 100.f);
        sprite.color = glm::vec4(100.f, 0.f, 100.f, 1.f);
        sprite.rotation_point = sprite.size / 2.f;
        sprite.rotation = glm::pi<float>() / 4.f;
        sprite.texture = &tex;
        sprite.texCoords = glm::ivec4(0, 0, tex.getSize());
        renderer_2D->render(sprite);

        Sprite ctr;
        ctr.position = sprite.position + sprite.rotation_point;
        ctr.size = glm::vec2(4.f, 4.f);
        renderer_2D->render(ctr);

        Texture tex2("res/block1_Jerom.png", true);
        sprite.texture = &tex2;
        sprite.position = glm::vec2(300.f, 100.f);
        renderer_2D->render(sprite);

        Font font = font_loader.loadFont("res/Inconsolata-Regular.ttf", 40);
        Text text(&font);
        text.position = glm::vec2(250, 300);
        text.text = "Hello World (OpenGL edition)!\nnew game coming";
        text.color = glm::vec4(0.f, 255.f, 0.f, 1.f);
        text.bloom = true;
        renderer_2D->render(text);

        Sprite bb_text;
        bb_text.color.a = 0.3f;
        bb_text.position = text.position;
        bb_text.size = text.getSize();
        bb_text.rotation = text.rotation;
        bb_text.rotation_point = text.rotation_point;
        renderer_2D->render(bb_text);
    }
    pp_unit->endRender();
    pp_unit->render();
    glfwSwapBuffers(window);
}

void App::set_opengl_states()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
