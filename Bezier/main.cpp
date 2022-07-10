#include "InputManager.h"
#include "glm/glm.hpp"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1400;
	const int DISPLAY_HEIGHT = 700;
	const float CAMERA_ANGLE = 60.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;
	const int infoIndx = 2; 
	std::list<int> x, y;
	x.push_back(DISPLAY_WIDTH / 2);
	//y.push_back(DISPLAY_HEIGHT / 2);
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)(DISPLAY_WIDTH / 2 / DISPLAY_HEIGHT), NEAR, FAR); // adding a camera
	Bezier *scn = new Bezier();  //initializing scene
	
	Init(display); //adding callback functions
	scn->Init();    //adding shaders, textures, shapes to scene
	rndr->Init(scn,x,y); // adding scene and viewports to the renderer
	display.SetRenderer(rndr);  // updating renderer in as a user pointer in glfw
	//Orthographic camera.
	rndr->AddCamera(glm::vec3(0, 0, 100), 0, (float)DISPLAY_WIDTH / 2 / DISPLAY_HEIGHT, NEAR, FAR, infoIndx);
	rndr->AddDraw(1, 1, 0, 0, 4 | 128 | 16);//2nd view picking
	rndr->AddViewport(0, 0, DISPLAY_WIDTH/2 , DISPLAY_HEIGHT);
	rndr->AddDraw(0, 0, 1, 0, rndr->depthTest | rndr->scaleStencil | rndr->stencilTest);//2nd phase stencil
	rndr->AddDraw(2, 0, 3, 0, rndr->inAction2 | rndr->scissorTest | rndr->blend);
	rndr->AddDraw(2, 0, 3, 0,  rndr->stencilTest | rndr->inAction3);//3rd phase stencil

	while(!display.CloseWindow())
	{
		rndr->DrawAll();
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();		
	}
	delete scn;
	return 0;
}
