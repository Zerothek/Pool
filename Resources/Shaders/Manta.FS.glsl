#version 330

uniform vec3 color;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4( 0.34902f, 0.19608f, 0.06667f, 1);
}