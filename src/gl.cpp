#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "Graphics/shader.hpp"
#include <Graphics/render.hpp>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(480, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    {
    Shader program("C:/Users/gaelm/Documents/C++ Projects/Graphics/res/shaders/test.glsl");
    // Shader program(__FILE__ "/../res/shaders/test.glsl");
    program.bind();

    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;

    std::cout << __FILE__ << std::endl;

    Point p1({-0.5, -0.5, 0.0});
    Point p2({0.5, -0.5, 0.0});
    Point p3({0.5,  0.5, 0.0});
    Point p4({-0.5, 0.5, 0.0});
    Point p5({0, 0.8,0});

    Surface s1({0.002,0.002,0}, {-0.001,-0.001,0}, p1, p4, p3, p2);
    Surface s2({0.002,0.002,0}, {-0.0015,-0.001,0}, p1, p4, p5);

    Surface pl1({0, 0.8, 0}, {0, 0.8, 0}, Point({-0.942167, -0.942167,  0.      }), Point({-0.942167,  0.057833,  0.      }), Point({ 0.057833,  0.057833,  0.}), Point({ 0.057833, -0.942167,  0.      }));
    Surface pl2({0, 0.8, 0}, {0, 0.8, 0}, Point({-1.54225 , -0.942167,  0.      }), Point({-1.54225 ,  0.057833,  0.      }), Point({-1.04225 ,  0.357833,  0.      }));

    std::cout << pl1.dist(pl2) << std::endl;

    // program.SetUniformf("u_Color", 0.2f,0.3f, 0.8f, 1.0f);

    int frames = 0;
    long long time = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        frames++;
        auto start = std::chrono::high_resolution_clock::now();
        /* Render here */
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        program.SetUniformf("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        s1.bind();
        s1.render();
        // program.SetUniformf("u_Color", 0.9f, 0.1f, 0.2f, 1.0f);
        // s2.bind();
        // s2.render();
        s1.update(1./60.);
        // s2.update(1./60.);

        // std::cout << s1.dist(s2) << "\n";
        // std::cout << s1 << "\n";
        // std::cout << s2 << "\n";

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        time += duration.count();
    }
    std::cout << "Frames: " << frames << std::endl;
    std::cout << "FPS: " << (frames*1000000000.)/time << std::endl;
    }
    glfwTerminate();
    return 0;
}
