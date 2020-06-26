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
#include "Graphics/camera.hpp"
#include "Graphics/gmath.hpp"
#include "Graphics/collision.hpp"

double xpos = 240, ypos = 240;
float dt;
bool start = true;
Camera cam{glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-90.f), glm::radians(-15.f)};
CHandler chandle;

void mouse_callback(GLFWwindow* window, double x, double y){
    if(start){
        xpos = x;
        ypos = y;
        start = false;
    }
    double dx = x - xpos, dy = ypos - y;
    xpos = x;
    ypos = y;
    cam.mouse_move((float)dx, (float)dy);
}

void scroll_callback(GLFWwindow* window, double dx, double dy){
    cam.mouse_scroll((float)dy);
}

void input(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.key_press(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.key_press(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.key_press(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.key_press(RIGHT, dt);
}

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
    int width = 480;
    int height = 480;
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
        int length = width < height ? width:height;
        glViewport((width - length)/2, (height - length)/2, length, length);
    });
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(0);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;

    GLCALL(glEnable(GL_DEPTH_TEST));
    {
    Shader program(PROJECT_DIR "/res/shaders/test.glsl");
    program.bind();
    std::cout << cam.front.x << ", " << cam.front.y << ", " << cam.front.z << std::endl;
    std::cout << sizeof(cam) << std::endl;

    Texture tex(PROJECT_DIR "/res/textures/wall.png");
    Texture tex2(PROJECT_DIR "/res/textures/emoji.png");
    tex.bind(0);
    tex2.bind(1);
    program.SetUniformi("Texture", 0);
    program.SetUniformi("Texture2", 1);


    Point p1({-0.5, 0.0, -0.5});
    Point p2({0.5, 0.0, -0.5});
    Point p3({0.5, 0.0,  0.5});
    Point p4({-0.5, 0.0, 0.5});
    Point p5({0, 0, 0.8});
    Point p6({0.5, 0.5, 0.0});

    Surface s1(glm::vec3(-5, 0.0, -5), glm::vec3(5, 0.0, -5), glm::vec3(5, 0.0,  5), glm::vec3(-5, 0.0, 5));
    s1.tex_coord(0, 0, 0);
    s1.tex_coord(1, 1, 0);
    s1.tex_coord(2, 1, 1);
    s1.tex_coord(3, 0, 1);
    s1.reload();

    Solid slope(glm::vec3(10,0,2), glm::vec3(10,0,-2), glm::vec3(20,0,2), glm::vec3(20,0,-2), glm::vec3(20,10,2), glm::vec3(20,10,-2));
    slope.tex_coord(0, 0, 0);
    slope.tex_coord(1, 1, 0);
    slope.tex_coord(2, 1, 1);
    slope.tex_coord(3, 0, 1);
    slope.tex_coord(4, 0, 0);
    slope.tex_coord(5, 1, 0);
    slope.reload();

    // s2.transform(glm::translate(glm::mat4(1.0), glm::vec3(2.0,-1.,0)));
    Surface s3(glm::vec3(10, -5, 2), glm::vec3(10, -5, -2), glm::vec3(10, 5, 2), glm::vec3(10, 5, -2));
    s3.tex_coord(0, 0, 0);
    s3.tex_coord(1, 1, 0);
    s3.tex_coord(2, 1, 1);
    s3.tex_coord(3, 0, 1);
    s3.reload();
    std::cout << s3.normVec() << std::endl;

    Solid sol = Solid(p1, p2, p3, p4, p6);
    sol.tex_coord(0, 0, 0);
    sol.tex_coord(1, 1, 0);
    sol.tex_coord(2, 1, 1);
    sol.tex_coord(3, 0, 1);
    sol.tex_coord(4, 0.5, 0.5);
    sol.reload();

    chandle.add(&s1, 1, true);
    chandle.add(&slope, 1, false);
    chandle.add(&sol, 1, false);

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
        input(window);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            sol.vel = glm::vec3(0, 3, 0);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            sol.vel = glm::vec3(0, -3, 0);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            sol.vel = glm::vec3(-3, 0, 0);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            sol.vel = glm::vec3(3, 0, 0);
        chandle();

        // cam.pitch += 0.1f;
        // cam.update();
        glm::mat4 project = glm::mat4(1.0);
        project = glm::perspective(cam.zoom, 1.0f, 0.1f, 100.0f);
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
        // s2.update(dt);
        // s2.model = glm::translate(s2.model, dt*glm::vec3(0.2,0.,0));

        // std::cout << s2 << "\t" << sol.dist(s2) <<"\n";
        // std::cout << sol.vel + slope.vel << "\r";
        // std::cout << s3.dist(sol) << "  \t";
        // std::cout << sol.dist(s3) << "\t\t\t\r";

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
