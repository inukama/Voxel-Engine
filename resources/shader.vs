#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUTPUT
{
    vec2 texCoord;
    vec3 fragCoord;
} OUT;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    OUT.fragCoord = gl_Position.xyz;
    OUT.texCoord = aTex;
}
