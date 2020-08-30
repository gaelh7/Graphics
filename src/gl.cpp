#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdlib>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include "Graphics/log.hpp"
#include "Graphics/input.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/georender.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/camera.hpp"
#include "Graphics/collision.hpp"
#include "Graphics/text.hpp"
#include "Graphics/window.hpp"
#include "Graphics/model.hpp"

    double xpos = 240, ypos = 240;

int main(void){
    std::atexit([](){
        std::cout << "Allocations: \t" << allocations << std::endl;
        std::cout << "Deletions: \t" << deletions << std::endl;
        std::cout << bytes_allocated << " Bytes allocated" << std::endl;
    });
    float dt{};
    gmh::Camera cam{glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-90.f), glm::radians(-15.f)};
    gmh::CHandler chandle(1);
    gmh::TextInput& textInp = gmh::TextInput::get();
    gmh::Window win(480, 480, "Gael's App");
    win.bind();
    win.setIcon(PROJECT_DIR "/res/textures/emoji.png");
    enableDebug();
    gmh::InputHandler inpHandle;
    inpHandle.set_cursor_pos([&win, &cam](double x, double y){
        if(glfwGetMouseButton(win.handle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            cam.rotate((float)(xpos - x), (float)(y - ypos));
        }
        xpos = x;
        ypos = y;
    });
    inpHandle.set_scroll([&cam](double dx, double dy){cam.zoom_out((float)dy);});
    inpHandle.bind_key(GLFW_KEY_ESCAPE, [&win](int mods){win.close();}, [](int mods){});
    inpHandle.bind_key(GLFW_KEY_W, [&cam](int mods){cam.set_dir(gmh::FORWARD);}, [&cam](int mods){cam.set_dir(gmh::NONE);});
    inpHandle.bind_key(GLFW_KEY_A, [&cam](int mods){cam.set_dir(gmh::LEFT);}, [&cam](int mods){cam.set_dir(gmh::NONE);});
    inpHandle.bind_key(GLFW_KEY_S, [&cam](int mods){cam.set_dir(gmh::BACKWARD);}, [&cam](int mods){cam.set_dir(gmh::NONE);});
    inpHandle.bind_key(GLFW_KEY_D, [&cam](int mods){cam.set_dir(gmh::RIGHT);}, [&cam](int mods){cam.set_dir(gmh::NONE);});
    glfwSwapInterval(0);
    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);
    {
    gmh::Font font = gmh::Font("C:/Windows/Fonts/arial.ttf", 48);
    gmh::Font::init(&win);
    gmh::Font font1 = gmh::Font("C:/Windows/Fonts/times.ttf", 48);
    gmh::Shader program(PROJECT_DIR "/res/shaders/test.glsl");
    // {
    //     gmh::Shader p(PROJECT_DIR "/res/shaders/test.glsl");
    //     program = p;
    // }
    program.bind();

    gmh::Texture tex(PROJECT_DIR "/res/textures/emoji.png");
    {
        gmh::Texture tex0(PROJECT_DIR "/res/textures/wall.png");
        tex = std::move(tex0);
    }
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



    gmh::Solid temp = gmh::Solid(glm::vec3(10,0,2), glm::vec3(10,0,-2), glm::vec3(20,0,2), glm::vec3(20,0,-2), glm::vec3(20,10,2), glm::vec3(20,10,-2));
    gmh::Solid slope = temp;
    slope = std::move(temp);
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
    // inpHandle.bind_key(GLFW_KEY_C, [&s1](int mods){s1.transform(glm::translate(glm::mat4(1), glm::vec3(0, 0.01, 0)));}, [](int mods){});

    gmh::Physical p1{&s1, 1, true};
    chandle.add(p1);
    chandle.add(&sol, 1);
    chandle.add(&slope, 5);


    // chandle.remove(&s1);
    // std::cout << sol.volume() << std::endl;
    std::string str = "This is text";
    glClearColor(1, 1, 1, 1);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(win.handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    gmh::Shader model_shader(PROJECT_DIR "/res/shaders/load_model.glsl");
    gmh::Model backpack(PROJECT_DIR "/res/models/backpack/backpack.obj");

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    glm::vec3 tcolor(0.0, 0.0, 0.0);

    /* Loop until the user closes the win.handle() */
    inpHandle.bind(win);
    textInp.bind(win);
    while (win.isOpen()){
        win.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chandle();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Debug window");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::ColorPicker3("Text Color", glm::value_ptr(tcolor));
        if(ImGui::Button("Bind text")) textInp.bind(win);
        if(ImGui::Button("Bind motion")) inpHandle.bind(win);
        ImGui::End();


        glm::mat4 project = glm::mat4(1.0);
        project = glm::perspective(cam.zoom, win.aspect(), 0.1f, 100.0f);
        tex.bind(0);
        tex2.bind(1);
        program.bind();
        program.SetUniformMatrixf<4, 4>("view", glm::value_ptr(cam.view()));
        program.SetUniformMatrixf<4, 4>("projection", glm::value_ptr(project));
        program.SetUniformMatrixf<4, 4>("model", sol.model_ptr());

        sol.bind();
        sol.render();

        program.SetUniformMatrixf<4, 4>("model", slope.model_ptr());
        slope.bind();
        slope.render();

        program.SetUniformMatrixf<4, 4>("model", s1.model_ptr());
        s1.bind();
        s1.render();


        model_shader.bind();
        model_shader.SetUniformMatrixf<4, 4>("view", glm::value_ptr(cam.view()));
        model_shader.SetUniformMatrixf<4, 4>("projection", glm::value_ptr(project));
        model_shader.SetUniformMatrixf<4, 4>("model", glm::value_ptr(glm::mat4(1)));
        backpack.render(model_shader);

        font1.bind();
        font1.render(textInp.text(), 190, 440, 1, tcolor);
        // font.render("Extra text", 190, 300, 1, tcolor);
        gmh::Font::unbind();
        cam.update(dt);
        sol.update(dt);
        s1.update(dt);
        slope.update(dt);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        dt = win.update();
    }
    std::cout << std::endl;
    std::cout << "Frames: " << win.current_frame() << std::endl;
    std::cout << "FPS: " << win.FPS() << std::endl;
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    gmh::Font::terminate();
    std::cout << "Press return to continue..." << std::flush;
    std::cin.get();
    return 0;
}
