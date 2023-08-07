#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	static inline const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, -1.0f), float pitch = 0.0f, float yaw = -90.0f, float fov = 90.0f)
		: m_Position(position), m_Pitch(pitch), m_Yaw(yaw), m_FOV(fov)
	{
		UpdateVectors();
	}

	// Returns the view matrix based on the camera's position, pitch, and yaw
	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Position + m_Front, WorldUp);
	}

	const glm::vec3& GetPosition() const
	{
		return m_Position;
	}

	void SetPosition(glm::vec3 position)
	{
		m_Position = position;
	}

	void Move(glm::vec3 position)
	{
		m_Position += position;
	}

	const float& GetPitch() const
	{
		return m_Pitch;
	}

	void PitchBy(float pitch, bool constrain = true)
	{
		m_Pitch += pitch;
		if (constrain)
		{
			if (m_Pitch < -90.0f)
				m_Pitch = -90.0f;
			if (m_Pitch > 90.0f)
				m_Pitch = 90.0f;
		}
		UpdateVectors();
	}

	const float& GetYaw() const
	{
		return m_Yaw;
	}

	void SetYaw(float yaw)
	{
		m_Yaw = yaw;
		UpdateVectors();
	}

	void YawBy(float yaw)
	{
		m_Yaw += yaw;
		UpdateVectors();
	}

	const glm::vec3& GetFront() const
	{
		return m_Front;
	}

	const glm::vec3& GetRight() const
	{
		return m_Right;
	}

	const glm::vec3& GetUp() const
	{
		return m_Up;
	}

	const float& GetFOV() const
	{
		return m_FOV;
	}

	void DecFOV(float fov, bool constrain = true)
	{
		m_FOV -= fov;
		if (constrain)
		{
			if (m_FOV > 90.0f)
				m_FOV = 90.0f;
			if (m_FOV < 45.0f)
				m_FOV = 45.0f;
		}
	}

private:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	float m_Pitch;
	float m_Yaw;
	float m_FOV;

private:
	void UpdateVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		m_Right = glm::normalize(glm::cross(m_Front, WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
};

#endif // CAMERA_H