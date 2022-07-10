#pragma once   //maybe should be static class
#include "display.h"
#include "renderer.h"
#include "game.h"


	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();

		if (action == GLFW_PRESS)
		{
			if (scn->emptyPicking())
			{
				double x2, y2;
				glfwGetCursorPos(window, &x2, &y2);
				if (rndr->Picking((int)x2, (int)y2) && !scn->isCubemap())
				{
					rndr->UpdatePosition(x2, y2);
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && x2 >= 700.0f)
						scn->straighten();
					
				}
				else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
					scn->convexHull(GLFW_MOUSE_BUTTON_LEFT, rndr->xrel, rndr->yrel, (float)x2, (float)y2);
				else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && x2 < 700)
				{
					rndr->UpdatePosition(x2, y2);
					rndr->Press(x2, y2);
				}
			}

		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			double x2, y2;
			glfwGetCursorPos(window, &x2, &y2);
			rndr->release();
			scn->releaseR();
			if(scn->isPicking())
				if(x2>=700)
					rndr->PickingRect(700, y2);
				else
					rndr->PickingRect(x2, y2);
			scn->correctPickingList();
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();

		double x2, y2;
		glfwGetCursorPos(window, &x2, &y2);
		//scn->MyTranslate(glm::vec3(0,0,xoffset),0);
		if (x2 >= 700.0f)
			scn->zoom(yoffset);
		else
			scn->zoom3d(yoffset);
		
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				//if (rndr->Picking((int)xpos, (int)ypos));
				if (xpos >= 700.0f) {
					scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT, rndr->xrel, rndr->yrel);
					scn->convexHull(GLFW_MOUSE_BUTTON_RIGHT, rndr->xrel, rndr->yrel, (float)xpos, (float)ypos);
				}
				else
				{
					scn->MouseProccessing3d(GLFW_MOUSE_BUTTON_RIGHT, rndr->xrel, rndr->yrel);
					//rndr->ActionDraw2();
				}
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				if(xpos>=700.0f)
					scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT, rndr->xrel, rndr->yrel);
				else
				{
					scn->MouseProccessing3d(GLFW_MOUSE_BUTTON_LEFT, rndr->xrel, rndr->yrel);
				}

			}
			else if (scn->isPicking())
			{
				scn->MouseProccessing3d(GLFW_MOUSE_BUTTON_RIGHT, rndr->xrel, rndr->yrel);
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

	void RotateCamera(GLFWwindow* window, int type, float angle)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();
		rndr->MoveCamera(0, scn->xTranslate, -scn->cameraPos.x);
		rndr->MoveCamera(0, scn->yTranslate, -scn->cameraPos.y);
		rndr->MoveCamera(0, scn->zTranslate, -scn->cameraPos.z);
		rndr->MoveCamera(0, type, angle);
		rndr->MoveCamera(0, type, angle);
		rndr->MoveCamera(0, scn->xTranslate, scn->cameraPos.x);
		rndr->MoveCamera(0, scn->yTranslate, scn->cameraPos.y);
		rndr->MoveCamera(0, scn->zTranslate, scn->cameraPos.z);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Bezier* scn = (Bezier*)rndr->GetScene();

		std::vector<glm::vec3> v = scn->calcDirections();
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_SPACE:
				scn->create3D();
				break;

			case GLFW_KEY_C:
				scn->Continuity();
				break;
			case GLFW_KEY_2:
				scn->reset(2);
				break;
			case GLFW_KEY_3:
				scn->reset(3);
				break;
			case GLFW_KEY_4:
				scn->reset(4);
				break;
			case GLFW_KEY_5:
				scn->reset(5);
				break;
			case GLFW_KEY_6:
				scn->reset(6);
				break;
			case GLFW_KEY_UP:
				RotateCamera(window, scn->yRotate, -scn->cameraAngle/2);
				RotateCamera(window, scn->xRotate, 0.4f);
				RotateCamera(window, scn->yRotate, scn->cameraAngle/2);
				scn->cameraAnglex += 0.8f;
				//scn->rotateBack(1);
				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				RotateCamera(window, scn->yRotate, -scn->cameraAngle/2);
				RotateCamera(window, scn->xRotate, -0.4f);
				RotateCamera(window, scn->yRotate, scn->cameraAngle/2);
				scn->cameraAnglex -= 0.8f;
				//scn->rotateBack(2);
				break;
			case GLFW_KEY_RIGHT:
				RotateCamera(window, scn->yRotate, -0.4f);
				scn->cameraAngle -= 0.8f;
				//scn->rotateBack(3);
				break;
			case GLFW_KEY_LEFT:
				RotateCamera(window, scn->yRotate, 0.4f);
				scn->cameraAngle += 0.8f;
				//scn->rotateBack(4);
				break;

			case GLFW_KEY_R:
				
				rndr->MoveCamera(0, scn->xTranslate, 0.25*v[0].x);
				rndr->MoveCamera(0, scn->zTranslate, 0.25*v[0].z);
				scn->cameraPos += glm::vec3(0.25*v[0].x, 0, 0.25*v[0].z);
				break;
			case GLFW_KEY_L:
				rndr->MoveCamera(0, scn->xTranslate, -0.25*v[0].x);
				rndr->MoveCamera(0, scn->zTranslate, -0.25*v[0].z);
				scn->cameraPos += glm::vec3(-0.25*v[0].x, 0, -0.25*v[0].z);
				break;
			case GLFW_KEY_U:
				rndr->MoveCamera(0, scn->xTranslate, 0.25*v[1].x);
				rndr->MoveCamera(0, scn->yTranslate, 0.25*v[1].y);
				rndr->MoveCamera(0, scn->zTranslate, 0.25*v[1].z);
				scn->cameraPos += glm::vec3(0.25*v[1].x, 0.25*v[1].y, 0.25*v[1].z);
				break;
			case GLFW_KEY_D:
				rndr->MoveCamera(0, scn->xTranslate, -0.25*v[1].x);
				rndr->MoveCamera(0, scn->yTranslate, -0.25*v[1].y);
				rndr->MoveCamera(0, scn->zTranslate, -0.25*v[1].z);
				scn->cameraPos += glm::vec3(-0.25*v[1].x, -0.25*v[1].y, -0.25*v[1].z);
				break;
			case GLFW_KEY_F:
				rndr->MoveCamera(0, scn->xTranslate, 0.25*v[2].x);
				rndr->MoveCamera(0, scn->yTranslate, 0.25*v[2].y);
				rndr->MoveCamera(0, scn->zTranslate, 0.25*v[2].z);
				scn->cameraPos += glm::vec3(0.25*v[2].x, 0.25*v[2].y, 0.25*v[2].z);
				break;
			case GLFW_KEY_B:
				rndr->MoveCamera(0, scn->xTranslate, -0.25*v[2].x);
				rndr->MoveCamera(0, scn->yTranslate, -0.25*v[2].y);
				rndr->MoveCamera(0, scn->zTranslate, -0.25*v[2].z);
				scn->cameraPos += glm::vec3(-0.25*v[2].x, -0.25*v[2].y, -0.25*v[2].z);
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

	