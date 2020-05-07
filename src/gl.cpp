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

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;

    GLCALL(glEnable(GL_DEPTH_TEST));
    {
    Shader program(PROJECT_DIR "/res/shaders/test.glsl");
    program.bind();

    Texture tex(PROJECT_DIR "/res/textures/wall.png");
    Texture tex2(PROJECT_DIR "/res/textures/emoji.png");
    tex.bind(0);
    tex2.bind(1);
    program.SetUniformi("Texture", 0);
    program.SetUniformi("Texture2", 1);


    Point p1({-0.5, -0.5, 0.0});
    Point p2({0.5, -0.5, 0.0});
    Point p3({0.5,  0.5, 0.0});
    Point p4({-0.5, 0.5, 0.0});
    Point p5({0, 0.8,0});
    Point p6({0, 0, 0.5});

    std::cout << sizeof(Surface) << std::endl;

    // trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

    // program.SetUniformMatrixf<4, 4>("trans", glm::value_ptr(trans1));

    Surface s1(p1, p3, p4, p2);
    Surface sLocal(s1.vertices);
    // s1.trans_mat = glm::translate(s1.trans_mat, glm::vec3(0.001, -0.002 ,0));
    // s1.transform();

    // s1.vertex_color(0, 1, 0, 0, 0);
    // s1.vertex_color(1, 0, 1, 0, 0.5);
    // s1.vertex_color(2, 0, 0, 1, 0);
    // s1.vertex_color(3, 1, 1, 1, 1);
    s1.tex_coord(0, 0, 0);
    s1.tex_coord(1, 1, 0);
    s1.tex_coord(2, 1, 1);
    s1.tex_coord(3, 0, 1);
    s1.reload();

    Surface s2(p1, p4, p5);
    s2.tex_coord(0, 0, 0);
    s2.tex_coord(1, 0, 0.8f);
    s2.tex_coord(2, 0.5, 1);
    s2.reload();

    // s2.transform(glm::translate(glm::mat4(1.0), glm::vec3(2.0,-1.,0)));

    Solid sol(p1, p2, p3, p4, p6);
    sol.tex_coord(0, 0, 0);
    sol.tex_coord(1, 1, 0);
    sol.tex_coord(2, 1, 1);
    sol.tex_coord(3, 0, 1);
    sol.tex_coord(4, 0.5, 0.5);
    sol.reload();
    sol.model = glm::rotate(sol.model,1.f, glm::vec3(1.0, 0., 0.));
    s2.model = glm::rotate(s2.model,1.f, glm::vec3(1.0, 0., 0.));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 project = glm::mat4(1.0);
    project = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    program.SetUniformMatrixf<4, 4>("view", glm::value_ptr(view));
    program.SetUniformMatrixf<4, 4>("projection", glm::value_ptr(project));

    auto strt = std::chrono::high_resolution_clock::now();
    std::cout << s2.dist(sol.world()) << std::endl;
    auto ed = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(ed - strt);
    std::cout << dur.count()/1000000000. << std::endl;


    int frames = 0;
    long long time = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        frames++;
        auto start = std::chrono::high_resolution_clock::now();
        /* Render here */
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // std::cout << Point(s1.local().pos).dist(Point(sLocal.pos)) << "\r";
        // std::cout << s1.local().equals(sLocal) << "\r";
        // std::cout << s1.local() << "\r";
        program.SetUniformMatrixf<4, 4>("model", glm::value_ptr(sol.model));
        view = glm::rotate(view, 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
        program.SetUniformMatrixf<4, 4>("view", glm::value_ptr(view));

        // s1.bind();
        // s1.render();
        // s1.transform();
        sol.bind();
        sol.render();

        program.SetUniformMatrixf<4, 4>("model", glm::value_ptr(s2.model));
        s2.bind();
        s2.render();
        s2.model = glm::translate(s2.model, glm::vec3(0.002,-0.001,0));

        // std::cout << s2 << "\t" << sol.dist(s2) <<"\n";
        // std::cout << s2.dist(Polyhedron(sol.world())) << "\n";


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
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
