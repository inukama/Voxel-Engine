#version 330 core
out vec4 FragColor;

uniform mat4 view;
uniform mat4 projection;

in VS_OUTPUT
{
    vec3 normal;
    vec3 fragCoord;
    vec3 aPos;
} IN;

#define LIGHT_SOURCE vec3(3.0, 9.0, 5.0)
#define LIGHT_COLOUR vec3(0.5, 0.7, 0.7)

void main()
{
    vec3 light_pos = (projection * vec4(LIGHT_SOURCE, 1.0)).xyz;
    //FragColor = vec4(IN.aPos, 1.0);
    vec3 light_dir = normalize(light_pos - IN.fragCoord);
    float diff = max(dot(IN.normal, light_dir), 0.0);
    vec3 diffuse = diff * LIGHT_COLOUR;
    vec3 result = 5.0 * diffuse / length(light_pos - IN.fragCoord);

    FragColor = vec4(result, 1.0);
} 