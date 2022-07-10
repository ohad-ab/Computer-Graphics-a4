#include "MovableGLM.h"
#include <stdio.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

MovableGLM::MovableGLM()
{
	ZeroTrans();
}

glm::mat4 MovableGLM::MakeTrans(const glm::mat4 &prevTransformations) const
{
	return prevTransformations * MakeTrans();
}

glm::mat4 MovableGLM::MakeTrans() const
{
	return    rot * trans* scl;
}

glm::mat4 MovableGLM::MakeTransNoScale() const
{
	return    rot * trans;
}

glm::mat4 MovableGLM::MakeTransNoRot() const
{
	return    trans * scl ;
}

void MovableGLM::MyTranslate(const glm::vec3 delta,int mode)
{
	trans = glm::translate(trans,glm::vec3(glm::transpose(rot) * glm::vec4(delta,0)));
}

void  MovableGLM::MyRotate(float angle,const glm::vec3 &vec,int mode)
{
	if (mode == 0)
		rot = glm::rotate(glm::mat4(1), angle, vec)*rot;
	else
		rot = glm::rotate(rot,angle,glm::vec3(glm::transpose(rot)*glm::vec4(vec,0)));
}
	
void  MovableGLM::MyScale(const glm::vec3 scale)
{
	scl = glm::scale(scl,scale);
}

void MovableGLM::setCenterOfRotation(glm::vec3 delta)
{
	transIn = glm::translate(transIn, -delta);
	transOut = glm::translate(transOut, delta);
}

glm::vec3 MovableGLM::getCenterOfRotation()
{
	return glm::vec3(transIn*glm::vec4(0, 0, 0, 1));
}

void MovableGLM::ZeroTrans()
{
	trans = glm::mat4(1);
	transIn = glm::mat4(1);
	transOut = glm::mat4(1);
	rot = glm::mat4(1);
	scl = glm::mat4(1);
}
