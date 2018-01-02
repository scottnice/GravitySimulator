#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/quaternion.hpp"

class Camera {
	glm::vec3 m_pos;
	glm::quat m_orient;
public:
	Camera() : m_pos(0, 0, 0), m_orient(1, 0, 0, 0) {}
	Camera(const Camera &) = default;

	Camera(const glm::vec3 &pos) : m_pos(pos), m_orient(1, 0, 0, 0) {}
	Camera(const glm::vec3 &pos, const glm::quat &orient) : m_pos(pos), m_orient(orient) {}

	Camera &operator =(const Camera &) = default;

	const glm::vec3 &position() const { return m_pos; }
	void setPosition(glm::vec3 const & pos) { m_pos = pos; }
	const glm::quat &orientation() const { return m_orient; }

	glm::mat4 view() const { return glm::translate(glm::mat4_cast(m_orient), m_pos); }

	void translate(const glm::vec3 &v) { m_pos += v * m_orient; }
	void translate(float x, float y, float z) { m_pos += glm::vec3(x, y, z) * m_orient; }

	void rotate(float angle, const glm::vec3 &axis) { m_orient *= glm::angleAxis(angle, axis * m_orient); }
	void rotate(float angle, float x, float y, float z) { m_orient *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orient); }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
};