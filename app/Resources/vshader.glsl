#version 330

uniform mat4 mvp_matrix;

in vec4 position;
in vec4 color;

out vec4 color_vert;

void main()
{
    color_vert = vec4(color.xyz, 0.3);
    gl_Position = mvp_matrix * position;
}