#pragma once
#include "scene.h"
#include "Bezier1D.h"

class Bezier : public Scene
{
public:
	
	Bezier();
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void MouseProccessing(int button, int xrel, int yrel);
	void convexHull(int button, int xrel, int yrel,float xpos, float ypos);
	void processAngle(int yrel);
	void releaseR();
	void releaseL();
	void straighten();
	void Continuity();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	
	unsigned int TextureDesine(int width, int height);
	Bezier1D* B;
	bool continuity;
	~Bezier(void);
};

