#pragma once
#include "glm/glm.hpp"

class MovableGLM 
{
public:
	MovableGLM();
	glm::mat4 MakeTrans(const glm::mat4 &prevTransformations) const;
	glm::mat4 MakeTrans() const;
	glm::mat4 MakeTransNoScale() const;
	glm::mat4 MakeTransNoRot() const;
	glm::mat4 getRot() const { return rot; }
	void MyTranslate(const glm::vec3 delta, int mode);
	void MyRotate(float angle, const glm::vec3 &vec, int mode);
	void MyScale(const glm::vec3 scale);
	void setCenterOfRotation(glm::vec3 delta);
	glm::vec3 getCenterOfRotation();
	void ZeroTrans();
private:
	glm::mat4 trans;
	glm::mat4 transIn;
	glm::mat4 transOut;
	glm::mat4 rot;
	glm::mat4 scl;
};
