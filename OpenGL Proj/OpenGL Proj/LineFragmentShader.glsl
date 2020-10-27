#version 430 core
in vec4 colours_export;
out vec4 colours_out;

void main(void)
{
	colours_out = colours_export;
}
