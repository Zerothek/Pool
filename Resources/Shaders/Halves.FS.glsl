#version 330

uniform vec3 color;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4( 1.0f, 0.0f, 1.0f, 1);
}