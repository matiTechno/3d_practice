#include "app.hpp"
#include <assert.h>
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <string>
// for testing
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

bool App::isCurrent  = false;

Key_input App::key_input;

void App::error_callback(int, const char* description)
{
    std::cout << "glfw error: " << description << std::endl;
}

void App::key_callback(GLFWwindow*, int key, int, int action, int)
{
    if(action == GLFW_PRESS)
        key_input.press_event(key);
    else if(action == GLFW_RELEASE)
        key_input.release_event(key);
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

    glfwSetKeyCallback(window, key_callback);

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
    renderer_3D = std::make_unique<Renderer_3D>();

    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        pp_unit = std::make_unique<Postprocessor>(width, height);
    }

    font = std::make_unique<Font>(font_loader.loadFont("res/Inconsolata-Regular.ttf", 40));
    texture = std::make_unique<Texture>("res/Candies_Jerom_CCBYSA3.png", true);
    model = std::make_unique<Model_3D>("res/monkey.obj");
    model->position.z = -3.f;

    set_opengl_states();
    run();
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
            //std::cout << "frametime: " << acc_time / static_cast<float>(frame_count) << '\r';
            //std::cout.flush();
            acc_time = 0.f;
            frame_count = 0;
        }

        update(frame_time);

        render();
    }
}

void App::processInput()
{
    key_input.start_new_frame();
    glfwPollEvents();

    if(key_input.was_pressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

void App::update(float dt)
{
    model->rotation.y += 2.f * glm::pi<float>() * dt / 30.f;

    frametime_panel = dt;

    if(key_input.is_pressed(GLFW_KEY_W))
        camera.move(Cam_move_dir::forward, dt);
    if(key_input.is_pressed(GLFW_KEY_S))
        camera.move(Cam_move_dir::backward, dt);
    if(key_input.is_pressed(GLFW_KEY_A))
        camera.move(Cam_move_dir::left, dt);
    if(key_input.is_pressed(GLFW_KEY_D))
        camera.move(Cam_move_dir::right, dt);
    if(key_input.is_pressed(GLFW_KEY_UP))
        camera.rotate(Cam_rotation_dir::up, dt);
    if(key_input.is_pressed(GLFW_KEY_DOWN))
        camera.rotate(Cam_rotation_dir::down, dt);
    if(key_input.is_pressed(GLFW_KEY_LEFT))
        camera.rotate(Cam_rotation_dir::left, dt);
    if(key_input.is_pressed(GLFW_KEY_RIGHT))
        camera.rotate(Cam_rotation_dir::right, dt);
}

#include "rendering/sprite.hpp"
#include "rendering/text.hpp"

void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    pp_unit->set_new_size(width, height);
    pp_unit->begRender();
    {
        renderer_3D->render(*model);

        {
            glm::mat4 proj = glm::ortho(0.f, static_cast<float>(width),
                                        static_cast<float>(height), 0.f);
            renderer_2D->load_projection(proj);
        }
        {
            glm::mat4 proj = glm::perspective(glm::pi<float>() / 2.f,
                                              static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.f);
            renderer_3D->load_projection(proj);
            renderer_3D->load_view_matrix(camera.get_view_matrix());
        }

        Sprite sprite;
        sprite.position = glm::vec2(100.f, 100.f);
        sprite.size = glm::vec2(100.f, 100.f);
        sprite.rotation_point = sprite.size / 2.f;
        sprite.rotation = glm::pi<float>() / 4.f;
        sprite.texture = texture.get();
        sprite.texCoords = glm::ivec4(0, 0, texture->getSize());
        renderer_2D->render(sprite);

        Sprite ctr;
        ctr.position = sprite.position + sprite.rotation_point;
        ctr.size = glm::vec2(4.f, 4.f);
        renderer_2D->render(ctr);

        Text text(font.get());
        text.position = glm::vec2(250, 500);
        text.text = "Hello World (OpenGL edition)!\nnew game coming";
        text.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
        text.bloom = true;

        Sprite bb_text;
        bb_text.color.a = 0.05f;
        bb_text.position = text.position;
        bb_text.size = text.getSize();
        bb_text.rotation = text.rotation;
        bb_text.rotation_point = text.rotation_point;
        renderer_2D->render(bb_text);

        renderer_2D->render(text);

        Text fps_panel(font.get());
        //fps_panel.set_pixel_size(25);
        fps_panel.position = glm::vec2(10.f, 10.f);
        fps_panel.text = "frametime: " + std::to_string(frametime_panel) + "\nfps: "
                + std::to_string(static_cast<int>(1.f / frametime_panel + 0.5f));
        fps_panel.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
        fps_panel.bloom = true;
        renderer_2D->render(fps_panel);
    }
    pp_unit->endRender();
    pp_unit->render(true);
    glfwSwapBuffers(window);
}

void App::set_opengl_states()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
