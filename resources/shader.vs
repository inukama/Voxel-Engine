#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUTPUT
{
    vec3 normal;
    vec3 fragCoord;
    vec3 aPos;
} OUT;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    OUT.fragCoord = gl_Position.xyz;
    OUT.normal = aNormal;
    OUT.aPos = aPos;
}
