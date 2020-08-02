#include <windows.h>
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

double xpos = 240, ypos = 240;
float dt;
gmh::Camera cam{glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-90.f), glm::radians(-15.f)};
gmh::CHandler chandle(1);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    int width = 480;
    int height = 480;
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;
    enableDebug();

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
        int length = width < height ? width:height;
        glViewport((width - length)/2, (height - length)/2, length, length);
    });
    gmh::InputHandler::init(window);
    gmh::InputHandler::set_cursor_pos([window](double x, double y){
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            cam.mouse_move((float)(xpos - x), (float)(y - ypos));
        }
        xpos = x;
        ypos = y;
    });
    gmh::InputHandler::set_scroll([](double dx, double dy){cam.mouse_scroll((float)dy);});
    gmh::InputHandler::bind_key(GLFW_KEY_ESCAPE, [&window](){glfwSetWindowShouldClose(window, true);}, [](){});
    gmh::InputHandler::bind_key(GLFW_KEY_W, [](){cam.set_dir(gmh::FORWARD);}, [](){cam.set_dir(gmh::NONE);});
    gmh::InputHandler::bind_key(GLFW_KEY_A, [](){cam.set_dir(gmh::LEFT);}, [](){cam.set_dir(gmh::NONE);});
    gmh::InputHandler::bind_key(GLFW_KEY_S, [](){cam.set_dir(gmh::BACKWARD);}, [](){cam.set_dir(gmh::NONE);});
    gmh::InputHandler::bind_key(GLFW_KEY_D, [](){cam.set_dir(gmh::RIGHT);}, [](){cam.set_dir(gmh::NONE);});
    glfwSwapInterval(0);
    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);
    {
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gmh::Font font = gmh::Font("C:/Windows/Fonts/times.ttf", 48);
    gmh::Shader program(PROJECT_DIR "/res/shaders/test.glsl");
    program.bind();
    std::cout << cam.front << std::endl;
    std::cout << sizeof(cam) << std::endl;

    gmh::Texture tex(PROJECT_DIR "/res/textures/wall.png");
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
    gmh::InputHandler::bind_key(GLFW_KEY_UP, [&sol](){sol.vel += glm::vec3(0, 3, 0);}, [&sol](){sol.vel = glm::vec3(0, 0, 0);});
    gmh::InputHandler::bind_key(GLFW_KEY_DOWN, [&sol](){sol.vel += glm::vec3(0, -3, 0);}, [&sol](){sol.vel = glm::vec3(0, 0, 0);});
    gmh::InputHandler::bind_key(GLFW_KEY_LEFT, [&sol](){sol.vel += glm::vec3(-3, 0, 0);}, [&sol](){sol.vel = glm::vec3(0, 0, 0);});
    gmh::InputHandler::bind_key(GLFW_KEY_RIGHT, [&sol](){sol.vel += glm::vec3(3, 0, 0);}, [&sol](){sol.vel = glm::vec3(0, 0, 0);});
    gmh::InputHandler::bind_key(GLFW_KEY_Z, [&sol](){sol.vel += glm::vec3(0, 0, 3);}, [&sol](){sol.vel = glm::vec3(0, 0, 0);});
    gmh::InputHandler::bind_key(GLFW_KEY_X, [&sol](){sol.vel += glm::vec3(0, 0, -3);}, [&sol](){sol.vel = glm::vec3(0, 0, 0);});
    gmh::InputHandler::bind_key(GLFW_KEY_SPACE, [&sol](){sol.vel += glm::vec3(0, 0, 0);}, [&sol](){sol.vel = glm::vec3(0, 0, 0);});
    gmh::InputHandler::bind_key(GLFW_KEY_C, [&s1](){s1.update(glm::translate(glm::mat4(1), glm::vec3(0, 0.01, 0)));}, [](){});

    chandle.add(&s1, 1, true);
    chandle.add(&sol, 1, false);
    chandle.add(&slope, 1, false);


    // chandle.remove(&s1);
    std::cout << sol.volume() << std::endl;

    int frames = 0;
    long long time = 0;

    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = MapVirtualKey(VK_UP, 0); // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    gmh::InputHandler::bind_key(GLFW_KEY_M, [&ip](){
            ip.ki.wVk = VK_UP; // virtual-key code for the "a" key
            ip.ki.dwFlags = KEYEVENTF_EXTENDEDKEY; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
        }, [&ip](){
            ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY;
            SendInput(1, &ip, sizeof(INPUT));
        });

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){

        frames++;
        auto start = std::chrono::high_resolution_clock::now();
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chandle();

        // cam.pitch += 0.1f;
        glm::mat4 project = glm::mat4(1.0);
        project = glm::perspective(cam.zoom, 1.0f, 0.1f, 100.0f);
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

        font.bind();
        font.render("This is text", 200, 200, 1, glm::vec3(1.0, 1.0, 1.0));

        // std::cout << sol.vel << "\t\t\t\t\r";
        cam.update(dt);
        sol.update(dt);
        s1.update(dt);
        slope.update(dt);
        // sol.vel -= dt*glm::vec3(0,1,0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        dt = duration.count()/1000000000.f;
        time += duration.count();
    }
    std::cout << std::endl;
    std::cout << "Frames: " << frames << std::endl;
    std::cout << "FPS: " << (frames*1000000000.)/time << std::endl;
    }
    glfwTerminate();
    std::cout << "Press return to continue...";
    std::cin.get();
    return 0;
}
