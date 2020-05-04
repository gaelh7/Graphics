#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 texCoord;


void main(){
   gl_Position = vec4(position, 1.0);
   vColor = aColor;
   texCoord = aTexCoord;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;
in vec4 vColor;
in vec2 texCoord;

uniform sampler2D Texture;

void main(){
    color = texture(Texture, texCoord) * vColor;
}
