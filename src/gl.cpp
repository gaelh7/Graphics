#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/log.hpp"
#include "Graphics/input.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/render.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/camera.hpp"
#include "Graphics/collision.hpp"
#include "Graphics/text.hpp"
#include "Graphics/window.hpp"

double xpos = 240, ypos = 240;
float dt;
gmh::Camera cam{glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-90.f), glm::radians(-15.f)};
gmh::CHandler chandle(1);

int main(void){
    gmh::TextInput& textInp = gmh::TextInput::get();
    gmh::Window win(480, 480, "Gael's App");
    win.setIcon(PROJECT_DIR "/res/textures/emoji.png");
    enableDebug();
    gmh::InputHandler inpHandle;
    inpHandle.set_cursor_pos([&win](double x, double y){
        if(glfwGetMouseButton(win.handle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            cam.mouse_move((float)(xpos - x), (float)(y - ypos));
        }
        xpos = x;
        ypos = y;
    });
    inpHandle.set_scroll([](double dx, double dy){cam.mouse_scroll((float)dy);});
    inpHandle.bind_key(GLFW_KEY_ESCAPE, [&win](int mods){glfwSetWindowShouldClose(win.handle(), true);}, [](int mods){});
    inpHandle.bind_key(GLFW_KEY_W, [](int mods){cam.set_dir(gmh::FORWARD);}, [](int mods){cam.set_dir(gmh::NONE);});
    inpHandle.bind_key(GLFW_KEY_A, [](int mods){cam.set_dir(gmh::LEFT);}, [](int mods){cam.set_dir(gmh::NONE);});
    inpHandle.bind_key(GLFW_KEY_S, [](int mods){cam.set_dir(gmh::BACKWARD);}, [](int mods){cam.set_dir(gmh::NONE);});
    inpHandle.bind_key(GLFW_KEY_D, [](int mods){cam.set_dir(gmh::RIGHT);}, [](int mods){cam.set_dir(gmh::NONE);});
    glfwSwapInterval(0);
    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);
    {
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gmh::Font font = gmh::Font("C:/Windows/Fonts/arial.ttf", 48);
    gmh::Font::init(&win);
    gmh::Font font1 = gmh::Font("C:/Windows/Fonts/times.ttf", 48);
    gmh::Shader program(PROJECT_DIR "/res/shaders/text.glsl");
    {
        gmh::Shader p(PROJECT_DIR "/res/shaders/test.glsl");
        program = p;
    }
    program.bind();
    std::cout << cam.front << std::endl;
    std::cout << sizeof(cam) << std::endl;

    gmh::Texture tex0(PROJECT_DIR "/res/textures/wall.png");
    gmh::Texture tex(PROJECT_DIR "/res/textures/emoji.png");
    tex = tex0;
    gmh::Texture tex2(PROJECT_DIR "/res/textures/emoji.png");
    tex.bind(0);
    tex2.bind(1);
    program.SetUniformi("Texture", 0);
    program.SetUniformi("Texture2", 1);


    gmh::Surface s1(glm::vec3(-5, 0.0, -5), glm::vec3(5, 0.0, -5), glm::vec3(5, 0.0,  5), glm::vec3(-5, 0.0, 5));
    s1.tex_coord(0, 0, 0);
    s1.tex_coord(1, 1, 0);
    s1.tex_coord(2, 1, 1);
    s1.tex_coord(3, 0, 1);
    s1.reload();



    gmh::Solid slope;
    slope = gmh::Solid(glm::vec3(10,0,2), glm::vec3(10,0,-2), glm::vec3(20,0,2), glm::vec3(20,0,-2), glm::vec3(20,10,2), glm::vec3(20,10,-2));
    slope.tex_coord(0, 0, 0);
    slope.tex_coord(1, 1, 0);
    slope.tex_coord(2, 1, 0);
    slope.tex_coord(3, 0, 0);
    slope.tex_coord(4, 1, 1);
    slope.tex_coord(5, 0, 1);
    slope.reload();

    gmh::Solid sol = gmh::Solid(glm::vec3(-0.5, 0.0, -0.5), glm::vec3(0.5, 0.0, -0.5), glm::vec3(0.5, 0.0,  0.5), glm::vec3(-0.5, 0.0, 0.5), glm::vec3(0.5, 0.5, 0.0));
    sol.tex_coord(0, 0, 0);
    sol.tex_coord(1, 1, 0);
    sol.tex_coord(2, 1, 1);
    sol.tex_coord(3, 0, 1);
    sol.tex_coord(4, 0.5, 0.5);
    sol.reload();
    inpHandle.bind_key(GLFW_KEY_UP, [&sol](int mods){sol.vel += glm::vec3(0, 3, 0);}, [&sol](int mods){sol.vel = glm::vec3(0, 0, 0);});
    inpHandle.bind_key(GLFW_KEY_DOWN, [&sol](int mods){sol.vel += glm::vec3(0, -3, 0);}, [&sol](int mods){sol.vel = glm::vec3(0, 0, 0);});
    inpHandle.bind_key(GLFW_KEY_LEFT, [&sol](int mods){sol.vel += glm::vec3(-3, 0, 0);}, [&sol](int mods){sol.vel = glm::vec3(0, 0, 0);});
    inpHandle.bind_key(GLFW_KEY_RIGHT, [&sol](int mods){sol.vel += glm::vec3(3, 0, 0);}, [&sol](int mods){sol.vel = glm::vec3(0, 0, 0);});
    inpHandle.bind_key(GLFW_KEY_Z, [&sol](int mods){sol.vel += glm::vec3(0, 0, 3);}, [&sol](int mods){sol.vel = glm::vec3(0, 0, 0);});
    inpHandle.bind_key(GLFW_KEY_X, [&sol](int mods){sol.vel += glm::vec3(0, 0, -3);}, [&sol](int mods){sol.vel = glm::vec3(0, 0, 0);});
    inpHandle.bind_key(GLFW_KEY_SPACE, [&sol](int mods){sol.vel += glm::vec3(0, 0, 0);}, [&sol](int mods){sol.vel = glm::vec3(0, 0, 0);});
    inpHandle.bind_key(GLFW_KEY_C, [&s1](int mods){s1.transform(glm::translate(glm::mat4(1), glm::vec3(0, 0.01, 0)));}, [](int mods){});

    chandle.add(&s1, 1, true);
    chandle.add(&sol, 1, false);
    chandle.add(&slope, 5, false);


    // chandle.remove(&s1);
    std::cout << sol.volume() << std::endl;
    std::string str = "This is text";
    // glClearColor(1, 1, 1, 1);
    // std::string buffer = "";
    // inpHandle.bind_key(GLFW_KEY_H, [&buffer](){buffer.push_back('h');}, [](){});
    // inpHandle.bind_key('\\', [&buffer](){buffer.push_back('\\');}, [](){});
    // inpHandle.bind_key(GLFW_KEY_ENTER, [&buffer](){buffer.push_back('\n');}, [](){});
    // inpHandle.bind_key(GLFW_KEY_BACKSPACE, [&buffer](){
    //     if(buffer.size() > 0)
    //         buffer.pop_back();
    // }, [](){});

    /* Loop until the user closes the win.handle() */
    // inpHandle.bind(win);
    textInp.bind(win);
    while (!glfwWindowShouldClose(win.handle())){

        // frames++;
        // auto start = std::chrono::high_resolution_clock::now();
        /* Render here */
        win.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chandle();

        // cam.pitch += 0.1f;
        glm::mat4 project = glm::mat4(1.0);
        project = glm::perspective(cam.zoom, win.aspect(), 0.1f, 100.0f);
        tex.bind(0);
        tex2.bind(1);
        program.bind();
        program.SetUniformMatrixf<4, 4>("view", glm::value_ptr(cam.view()));
        program.SetUniformMatrixf<4, 4>("projection", glm::value_ptr(project));
        program.SetUniformMatrixf<4, 4>("model", glm::value_ptr(sol.model));

        // s1.bind();
        // s1.render();
        // s1.transform();
        sol.bind();
        sol.render();

        program.SetUniformMatrixf<4, 4>("model", glm::value_ptr(slope.model));
        slope.bind();
        slope.render();

        program.SetUniformMatrixf<4, 4>("model", glm::value_ptr(s1.model));
        s1.bind();
        s1.render();

        font1.bind();
        font1.render(textInp.text(), 190, 440, 1, glm::vec3(0.2, 0.0, 0.7));
        gmh::Font::unbind();
        // std::cout << sol.vel << "\t\t\t\t\r";
        cam.update(dt);
        sol.update(dt);
        s1.update(dt);
        slope.update(dt);
        dt = win.update();
    }
    std::cout << std::endl;
    std::cout << "Frames: " << win.current_frame() << std::endl;
    std::cout << "FPS: " << win.FPS() << std::endl;
    }
    glfwTerminate();
    gmh::Font::terminate();
    std::cout << "Press return to continue...";
    std::cin.get();
    return 0;
}
