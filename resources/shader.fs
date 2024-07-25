#version 330 core
out vec4 FragColor;

uniform sampler2D block_texture;

in VS_OUTPUT
{
    vec2 texCoord;
    vec3 fragCoord;
} IN;

void main()
{
    //FragColor = vec4(IN.texCoord, 0.0, 1.0);
    FragColor = texture(block_texture, IN.texCoord);
} 