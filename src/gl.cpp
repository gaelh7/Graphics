#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "Graphics/shader.hpp"
#include "Graphics/render.hpp"
#include "Graphics/texture.hpp"

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
    Shader program(PROJECT_DIR "/res/shaders/test.glsl");
    // Shader program(__FILE__ "/../res/shaders/test.glsl");
    program.bind();

    Texture tex(PROJECT_DIR "/res/textures/wall.png");
    tex.bind(0);
    program.SetUniformi("Texture", 0);

    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;

    Point p1({-0.5, -0.5, 0.0});
    Point p2({0.5, -0.5, 0.0});
    Point p3({0.5,  0.5, 0.0});
    Point p4({-0.5, 0.5, 0.0});
    Point p5({0, 0.8,0});

    std::cout << sizeof(glm::vec3) << std::endl;

    // trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

    // program.SetUniformMatrixf<4, 4>("trans", glm::value_ptr(trans1));

    Surface s1(p1, p3, p4, p2);
    s1.trans_mat = glm::rotate(s1.trans_mat, glm::radians(1.0f), (glm::vec3)s1.normVec());

    // s1.vertex_color(0, 1, 0, 0, 0);
    // s1.vertex_color(1, 0, 1, 0, 0.5);
    // s1.vertex_color(2, 0, 0, 1, 0);
    // s1.vertex_color(3, 1, 1, 1, 1);
    s1.tex_coord(0, 0, 0);
    s1.tex_coord(1, 0, 1);
    s1.tex_coord(2, 1, 1);
    s1.tex_coord(3, 0, 1);
    s1.reload();

    Surface s2(p1, p4, p5);
    s2.tex_coord(0, 0, 0);
    s2.tex_coord(1, 0, 0.8);
    s2.tex_coord(2, 0.5, 1);
    s2.reload();
    s2.trans_mat = glm::translate(s2.trans_mat, glm::vec3(-0.004,-0.001,0));
    // s2.trans_mat = &s2_mat;
    // s2.vel = {0.002,0.002,0};
    // s2.acc = {-0.0015,-0.001,0};

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

        // trans1 = *s1.trans_mat*trans1;
        program.SetUniformMatrixf<4, 4>("trans", glm::value_ptr(s1.tot_change));

        s1.bind();
        s1.render();
        s1.transform();

        // trans2 = *s2.trans_mat*trans2;
        program.SetUniformMatrixf<4, 4>("trans", glm::value_ptr(s2.tot_change));
        s2.bind();
        s2.render();
        s2.transform();

        std::cout << s1.dist(s2) << "\n";
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
