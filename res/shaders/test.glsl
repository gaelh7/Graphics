#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 texCoord;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main(){
   gl_Position = projection * view * model * vec4(position, 1.0);
   vColor = aColor;
   texCoord = aTexCoord;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;
in vec4 vColor;
in vec2 texCoord;

uniform sampler2D Texture;
uniform sampler2D Texture2;

void main(){
    color = mix(texture(Texture, texCoord), texture(Texture2, texCoord), 0.4);
}
