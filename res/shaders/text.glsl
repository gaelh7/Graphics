#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 projection = mat4(1.0);

void main(){
    gl_Position = projection*vec4(pos, 0.0, 1.0);
    texCoords = aTexCoords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 texCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main(){
    color = vec4(textColor, texture(text, texCoords).r);
}
