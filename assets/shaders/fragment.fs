#version 330 core
out vec4 FragColor;  

uniform vec3 uniColor;

void main()
{
    FragColor = vec4(uniColor, 1.0);
}