#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 position;
layout(location = 3) in vec3 normal;
layout(location = 2) in vec3 texture;
layout(location = 1) in vec3 vertex_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform float Angle;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_position;
out float frag_time;

void main()
{
	// TODO: send output to fragment shader
	frag_color = vertex_color;
	frag_normal = normal;
	vec4 nou = Model * vec4(position, 1.0) + vec4((1 - abs(cos(Time))) / 1.5f * sin(Angle) ,(1 - abs(cos(Time))) / 15, (1 - abs(cos(Time))) / 1.5f * cos(Angle), 0);
	//gl_Position =   Projection  * View  *  Model * vec4(position, 1.0);
	gl_Position =   Projection  * View  *  nou;

}
