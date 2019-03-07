#version 330

// TODO: get values from fragment shader

in vec3 frag_normal;
in vec3 frag_color;
in float frag_time;

layout(location = 0) out vec4 out_color;

uniform float Time;

in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 eye_position;


uniform vec3 object_color;


void main()
{
	vec3 light_position = vec3(0, 1, 0);
	vec3 light_direction =vec3(0, -1, 0);
	float material_shininess = 30;
	float material_kd = 0.5;
	float material_ks = 0.5;


	vec3 w_normal = normalize(world_normal);

	vec4 light_pos1 = vec4(light_position,1);

	vec3 light_pos = vec3(light_pos1.x, light_pos1.y, light_pos1);
	vec3 L = normalize( light_pos - world_position);
	vec3 V = normalize( eye_position - world_position);
	vec3 H = normalize( L + V );

	// TODO: define ambient light component
	float ambient_light = material_kd * 0.50;

	// TODO: compute diffuse light component
	float diffuse_light = max(dot(L, w_normal), 0) * material_kd;

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light =  material_ks * pow(max(dot(w_normal, H), 0), material_shininess);
	}

	// TODO: compute light

	//Spotlight
	float cut_off = radians(80);
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);
	float I;
	if (spot_light > cos(cut_off))
	{
		// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
		// se calculeaza atenuarea luminii
		float d = distance(light_pos, world_position);
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);
		I = ambient_light + (diffuse_light + specular_light)*light_att_factor;
	} else {
		I = ambient_light;
	}


	// TODO: write pixel out color
	out_color = vec4(0.05f, 0.05f, 0.05f, 1) * I;

}