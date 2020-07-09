#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vert {
	glm::vec3 pos;
	glm::vec3 normal;
};

class Buffer {
private:
	unsigned int m_id;
public:
	inline Buffer() {
		glCreateBuffers(1, &m_id);
	}
	inline ~Buffer() {
		glDeleteBuffers(1, &m_id);
	}
	inline void uploadData(const void* data, unsigned int size) {
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}
	inline unsigned int getId() {
		return m_id;
	}
};