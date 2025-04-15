#version 330 core
out vec4 FragColor;
in vec3 ourColor;

uniform vec2 mousePos;
uniform int time;

void main()
{
    vec4 c1 = vec4(sin(time*0.005f),cos(time*0.005f), 0.5f, 1.0f);
    vec4 c2 = vec4(ourColor, 1.0);
    vec4 c3 = vec4(ourColor.x*sin(time*0.0005f), 1.0f - ourColor.y, ourColor.z, 0.4f);
    FragColor = mix(c1, c2,c3);
} 