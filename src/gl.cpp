#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <render.hpp>

#define ASSERT(x) if ((!x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError(){
    while(glGetError());
}

static bool GLLogCall(const char* function, const char* file, int line){
    if(GLenum error = glGetError()){
        std::cout << "OpenGLError " << std::hex << error << ": " << file << "(" << std::dec << line << "): in " << function << std::endl;
        return false;
    }
    return true;
}

struct ShaderSource{
    std::string VertexShader;
    std::string FragmentShader;
};

static ShaderSource ParseShader(const std::string &filepath){
    std::ifstream stream(filepath);

    enum ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = NONE;

    while(getline(stream, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos)
                type = VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = FRAGMENT;
        }
        else{
            ss[type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(const std::string &source, unsigned int type){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int succeed;
    glGetShaderiv(id, GL_COMPILE_STATUS, &succeed);

    if(!succeed){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader: ";
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShaders(const ShaderSource prg){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(prg.VertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(prg.FragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    ShaderSource src = ParseShader("../res/shaders/test.glsl");
    unsigned int shader = CreateShaders(src);
    glUseProgram(shader);

    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;

    float vec[] = {
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0,
         0.5,  0.5, 0.0,
        -0.5, 0.5, 0.0,
    };

    std::cout << sizeof(vec) << std::endl;

    unsigned int ind[] = {
        0, 1, 2,
        0, 2, 3
    };

    // unsigned int VBO;
    // glGenBuffers(1, &VBO);

    // unsigned int IBO;
    // glGenBuffers(1, &IBO);

    // GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    // GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vec), &vec, GL_STATIC_DRAW));

    // GLCALL(glEnableVertexAttribArray(0));
    // GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, 3*sizeof(float), 0));

    // GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    // GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), &ind, GL_STATIC_DRAW));
    Point p1({-0.5, -0.5, 0.0});
    Point p2({0.5, -0.5, 0.0});
    Point p3({0.5,  0.5, 0.0});
    Point p4({-0.5, 0.5, 0.0});

    Surface s1(p1, p2, p3, p4);
    s1.bind();
    int frames = 0;
    long long time = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        frames++;
        auto start = std::chrono::high_resolution_clock::now();
        /* Render here */
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // GLClearError();
        s1.render();
        // GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        // ASSERT(GLLogCall());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        time += duration.count();
    }
    std::cout << "FPS: " << (frames*1000000000.)/time << std::endl;

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
