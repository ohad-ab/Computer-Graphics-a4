#pragma once
#include "scene.h"
#include "Bezier1D.h"

class Bezier : public Scene
{
public:
	
	Bezier();
	void Init();
	void Update(const glm::mat4 &proj, glm::mat4 &view,const glm::mat4 &Model,const int  shaderIndx);
	void MouseProccessing(int button, int xrel, int yrel);
	void MouseProccessing3d(int button, int xrel, int yrel);
	void convexHull(int button, int xrel, int yrel,float xpos, float ypos);
	void processAngle(int yrel);
	void releaseR();
	void releaseL();
	void straighten();
	void Continuity();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	void reset(int num);
	void zoom(int dir);
	void zoom3d(int dir);
	void createShapes();
	void create3D();
	bool isCubemap() { return (picked == B->GetSegmentsNum() * 3 + 4); }
	std::vector<glm::vec3> calcDirections();
	void correctPickingList();
	unsigned int TextureDesine(int width, int height);
	Bezier1D* B;
	bool continuity;
	glm::vec3 cameraPos;
	//float cameraAngle;
	~Bezier(void);

private:
	float scale;

};

