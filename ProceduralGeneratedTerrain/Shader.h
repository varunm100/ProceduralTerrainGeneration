#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderFileParser.h"

class Shader {
private:
	int m_Program;
public:
	Shader(const char* src) { 
		string a, b;
		ShaderFileParser::getShaderSrc(src, a, b);
		const char* vs_src;
		const char* fs_src;
		vs_src = a.c_str();
		fs_src = b.c_str();
		int shaders[3];
		char infoLog[512];
		shaders[0] = glCreateShader(GL_VERTEX_SHADER);
		shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shaders[0], 1, &vs_src, nullptr);
		glShaderSource(shaders[1], 1, &fs_src, nullptr);
		glCompileShader(shaders[0]);
		glCompileShader(shaders[1]);
		glGetShaderiv(shaders[0], GL_COMPILE_STATUS, &shaders[2]);
		if (!(shaders[2])) {
			glGetShaderInfoLog(shaders[0], 512, nullptr, infoLog);
			printf("Vertex Shader Compilation Failed: %s\n", infoLog);
		}
		glGetShaderiv(shaders[1], GL_COMPILE_STATUS, &shaders[2]);
		if (!(shaders[2])) {
			glGetShaderInfoLog(shaders[1], 512, nullptr, infoLog);
			printf("Fragment Shader Compilation Failed: %s\n", infoLog);
		}
		m_Program = glCreateProgram();
		glAttachShader(m_Program, shaders[0]);
		glAttachShader(m_Program, shaders[1]);
		glLinkProgram(m_Program);
		glGetProgramiv(m_Program, GL_LINK_STATUS, &shaders[2]);
		if (!(shaders[2])) {
			glGetProgramInfoLog(m_Program, 512, nullptr, infoLog);
			printf("Linking Shader Failed: %s\n", infoLog);
		}
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
	}
	inline ~Shader() { glDeleteProgram(m_Program); }
	inline void use() { glUseProgram(m_Program); }

	inline void setMat4(const char* uniform, glm::mat4& matrix) { glUniformMatrix4fv(glGetUniformLocation(m_Program, uniform), 1, GL_FALSE, &matrix[0][0]); }
	inline void setFloat(const char* uniform, float& f) { glUniform1f(glGetUniformLocation(m_Program, uniform),  f); }
};