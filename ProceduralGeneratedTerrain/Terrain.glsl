#SHADER VERTEX
#version 450 core

layout(location = 0)  in vec3 position;
layout(location = 1)  in vec3 normal;

struct MVT {
	mat4 model;
	mat4 view;
	mat4 projection;
};

struct DirLight {
	vec3 dir;

	vec3 ambient;
	vec3 diffuse;
};

uniform MVT uMVT;
uniform vec3 terrainDiffuse;
uniform DirLight light;

out vec3 color;

vec3 CalcDirLight(DirLight light, vec3 normal);

void main() {
	gl_Position = uMVT.projection*uMVT.view*uMVT.model*vec4(position, 1.0);
	color = CalcDirLight(light, normal);
};


vec3 CalcDirLight(DirLight light, vec3 normal) {
	vec3 lightDir = normalize(light.dir);
	float diff = max(dot(normal, lightDir), 0.0f);
	float ambientStrength = 0.5f;
	vec3 diffuse = diff * light.diffuse * terrainDiffuse;
	vec3 ambient = terrainDiffuse * light.ambient;

	vec3 result = ambient + diffuse;
	return result;
}

#SHADER FRAGMENT
#version 450 core

in vec3 color;
out vec4 fragColor;

void main() {
	fragColor = vec4(color, 1.0f);
};
