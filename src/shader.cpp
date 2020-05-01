#include <Graphics/shader.hpp>

Shader::Shader(const char* filepath): src(ParseShader(filepath)), path(filepath), id(CreateShaders(src)){}

Shader::~Shader(){
    GLCALL(glDeleteProgram(id));
}

ShaderSource Shader::ParseShader(const char* filepath){
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

unsigned int Shader::CompileShader(unsigned int type, const std::string &source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

    int succeed;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &succeed));

    if(!succeed){
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCALL(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile shader: ";
        std::cout << message << std::endl;
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShaders(const ShaderSource prg){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, prg.VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, prg.FragmentShader);

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));
    GLCALL(glDetachShader(program, vs));
    GLCALL(glDetachShader(program, fs));

    return program;
}
