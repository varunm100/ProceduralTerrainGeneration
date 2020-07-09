#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>
class Camera
{
public:
	float m_Yaw = 0.0f, m_Pitch = 0.0f, prevX =  1920/2, prevY = 1080/2;
	glm::vec3 m_Pos, m_Up = glm::vec3(0.0f, 1.0f, 0.0f), m_Dir;
	glm::mat4 view;
	bool first = true;

	const float rotSpeed = 0.045f;
	const float moveSpeed = 3.5f;
	inline Camera(glm::vec3 initialPos, glm::vec3 dir) : m_Pos { initialPos }, m_Dir{ dir } {
		view = glm::lookAt(m_Pos, glm::vec3(0.0f, 0.0f, 0.0f), m_Up);
	}
	inline void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
		if (first) {
			prevX = xpos;
			prevY = ypos;
			first = false;
		}
		float dx = xpos - prevX;
		float dy = -ypos + prevY;
		prevX = xpos;
		prevY = ypos;
		m_Yaw += (dx * rotSpeed);
		m_Pitch += (dy * rotSpeed);
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
		m_Dir = glm::normalize(glm::vec3(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)), sin(glm::radians(m_Pitch)), sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))));
		view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
	}
	inline void checkInput(GLFWwindow* window, float dt) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Pos += m_Dir * moveSpeed * dt;
			view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Pos -= m_Dir * moveSpeed * dt;
			view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Pos += glm::normalize(glm::cross(m_Up, m_Dir)) * moveSpeed * dt;
			view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Pos -= glm::normalize(glm::cross(m_Up, m_Dir)) * moveSpeed * dt;
			view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			m_Pos += glm::normalize(m_Up) * moveSpeed * dt;
			view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			m_Pos -= glm::normalize(m_Up) * moveSpeed * dt;
			view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
		
	}
	~Camera() { }
};

