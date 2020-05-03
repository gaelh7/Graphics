#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 texCoord;

out vec4 vColor;

void main(){
   gl_Position = vec4(position, 1.0);
   vColor = aColor;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;
in vec4 vColor;

void main(){
    color = vColor;
}
