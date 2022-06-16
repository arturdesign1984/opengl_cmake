#version 460

in vec3 color;

out vec4 fragColor;

uniform vec3 setColor;

void main()
{
    fragColor = vec4(color, 1.0) * vec4(setColor, 1.0);
}