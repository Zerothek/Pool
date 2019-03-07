#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

out vec3 world_position;
out vec3 world_normal;

void main()
{
	frag_normal = v_normal;
	frag_color = v_color;
	tex_coord = v_texture_coord;

	// TODO: compute world space vertex position and normal
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
	vec3 world_norm = normalize(mat3(Model) * v_normal);
	
	
	// TODO: send world position and world normal to Fragment Shader
	world_position = world_pos;
	world_normal = world_norm;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);

}
