#pragma once   //maybe should be static class
#include "display.h"
#include "renderer.h"
#include "game.h"


	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		
		if(action == GLFW_PRESS )
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			Bezier* scn = (Bezier*)rndr->GetScene();
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
			if (rndr->Picking((int)x2, (int)y2))
			{
				rndr->UpdatePosition(x2, y2);
				if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
					scn->straighten();
			}
			else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				scn->convexHull(GLFW_MOUSE_BUTTON_LEFT, rndr->xrel, rndr->yrel, (float)x2, (float)y2);

		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();

		scn->MyTranslate(glm::vec3(0,0,xoffset),0);
		
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				//if (rndr->Picking((int)xpos, (int)ypos));
					scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT,rndr->xrel,rndr->yrel);
					scn->convexHull(GLFW_MOUSE_BUTTON_RIGHT,rndr->xrel, rndr->yrel,(float)xpos, (float)ypos);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT, rndr->xrel, rndr->yrel);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
			{
				scn->releaseR();
			}
			

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		
		rndr->Resize(width,height);
		
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();

		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_SPACE:
				if (scn->IsActive())
					scn->Deactivate();
				else
					scn->Activate();
				break;

			case GLFW_KEY_C:
				scn->Continuity();
				break;
			case GLFW_KEY_UP:
				rndr->MoveCamera(0, scn->zTranslate, 0.4f);
				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				rndr->MoveCamera(0, scn->zTranslate, -0.4f);
				break;

			default:
				break;
			}
		}
	}

	void Init(Display &display)
	{
		display.AddKeyCallBack(key_callback);
		display.AddMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.AddResizeCallBack(window_size_callback);
	}
