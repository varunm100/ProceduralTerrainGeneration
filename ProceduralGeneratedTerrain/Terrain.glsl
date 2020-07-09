#SHADER VERTEX
#version 450 core

layout(location = 0)  in vec3 position;
layout(location = 1)  in vec3 normal;

struct MVT {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform MVT uMVT;

void main() {
	gl_Position = uMVT.projection*uMVT.view*uMVT.model*vec4(position, 1.0);
};

#SHADER FRAGMENT
#version 450 core

out vec4 fragColor;

void main() {
	fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};