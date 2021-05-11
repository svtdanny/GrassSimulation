#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texturePos;

out vec2 TexCoord;

uniform float time;
uniform mat4 PV;

void main()
{
   gl_Position = PV * vec4(aPos.x + 0.1*sin(time) * (aPos.y), aPos.y, aPos.z, 1.0);
   TexCoord = texturePos;
}