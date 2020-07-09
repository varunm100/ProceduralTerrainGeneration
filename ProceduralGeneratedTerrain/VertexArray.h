#pragma once
#include "glad/glad.h"
#include "Buffer.h"

class VertexArray
{
private:
	unsigned int m_id;
public:
	inline VertexArray() { glCreateVertexArrays(1, &m_id); }

	inline void addBuffer(unsigned int id, int typeOfBuffer, GLsizei stride) {
		// 0 = vertexbuffer, 1 = elementbuffer
		if (!typeOfBuffer)
			glVertexArrayVertexBuffer(m_id, 0, id, 0, stride);
		else
			glVertexArrayElementBuffer(m_id, id);
	}
	inline void addBuffers(Buffer* vb, Buffer* ib, GLsizei stride) {
		glVertexArrayVertexBuffer(m_id, 0, vb->getId(), 0, stride);
		glVertexArrayElementBuffer(m_id, ib->getId());
	}
	inline void addAttrib(const unsigned int index, const int count, const unsigned int offset) {
		glEnableVertexArrayAttrib(m_id, index);
		glVertexArrayAttribFormat(m_id, index, count, GL_FLOAT, GL_FALSE, offset);
		glVertexArrayAttribBinding(m_id, index, 0);
	}
	inline void bind() {
		glBindVertexArray(m_id);
	}

	~VertexArray() { glDeleteVertexArrays(1, &m_id); }
};

