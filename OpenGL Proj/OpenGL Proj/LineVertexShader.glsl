#version 430 core

layout(location = 0) in vec4 vertex_coords;
uniform mat4 line_projection_matrix;
uniform mat4 line_model_matrix;
uniform mat4 line_view_matrix;

out vec4 colours_export;

void main(void)
{
	gl_Position = line_projection_matrix * line_view_matrix * line_model_matrix * vertex_coords;
	colours_export = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

