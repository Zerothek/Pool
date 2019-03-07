#version 330

// TODO: get values from fragment shader

in vec3 frag_normal;
in vec3 frag_color;
in float frag_time;

layout(location = 0) out vec4 out_color;

uniform float Time;

void main()
{
	
	// TODO: write pixel out color
	 out_color = vec4( 0.96471f, 0.78824f, 0.53333f, 1);
}