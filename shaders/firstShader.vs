#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 color;


uniform vec3 setColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = vertexColor * setColor;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}