#include "game.h"
#include <iostream>
#include "GL/glew.h"
#include "Bezier1D.h"
#include "Bezier2D.h"
#include <math.h>
#include "glm/gtc/matrix_transform.hpp"

const float pi = 2 * acos(0);

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Bezier::Bezier() : Scene()
{
	continuity = false;
	scale = 0.2;
	cameraPos = glm::vec3(0, 0, 10);
}
//Game::Game(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

void Bezier::Init()
{		
	unsigned int texIDs[3] = { 0 , 1, 0};
	unsigned int slots[3] = { 0 , 1, 0 };
	
	AddShader("../res/shaders/pickingShader");	
	//AddShader("../res/shaders/basicShader2");
	AddShader("../res/shaders/basicShader");
	AddShader("../res/shaders/greenShader");
	AddShader("../res/shaders/cubeShader");
	AddShader("../res/shaders/lightShader");

	AddTexture("../res/textures/box0.bmp",2);
	AddTexture("../res/textures/cubemap/", 3);
	

	AddMaterial(texIDs,slots, 2);
	AddMaterial(texIDs+1,slots+1, 1);
	AddShape(Plane, -2, TRIANGLES);//scissor & stencil plane
	AddShapeViewport(0, 2);
	RemoveShapeViewport(0, 0);
	SetShapeShader(0, 2);
	pickedShape = 0;
	ShapeTransformation(zTranslate, -1.1f);
	ShapeTransformation(xTranslate, 1.0f);
	AddShape(Axis, -1, LINES);
	RemoveShapeViewport(1, 0);
	AddShapeViewport(1, 1);
	B = new Bezier1D(1, 20, LINE_STRIP);
	chainParents.push_back(-1);
	shapes.push_back(B);
	AddShapeViewport(2, 1);
	RemoveShapeViewport(2, 0);

	pickedShape = 0;
	SetShapeMaterial(0, 0);
	ShapeTransformation(xTranslate, -1);

	pickedShape = 1;
	shapes[pickedShape]->MyScale(glm::vec3(10,10,10));
	
	shapes[2]->MyScale(glm::vec3(0.2, 0.2, 0.2));
	
	pickedShape = -1;
	createShapes();//create the octahedrons
	
	AddShape(Cube, -1, TRIANGLES);//cubemap
	SetShapeMaterial(shapes.size() - 1, 1);
	SetShapeShader(shapes.size() - 1, 3);
	pickedShape = 13;
	shapes[shapes.size() - 1]->MyScale(glm::vec3(50, 50, 50));
	
}

void Bezier::createShapes()
{
	int ind = 3;
	for (int i = 0; i < B->GetSegmentsNum(); i++)
		for (int j = 0; j < 3 || (j == 3 && i == B->GetSegmentsNum() - 1); j++)//for every ctrl pt in segment including the last one
		{
			int parent = -1;
			float dx = 0, dy = 0;
			pickedShape = ind;
			glm::vec4 p = B->GetControlPoint(i, j);
			if (pickedShape % 3 == 0)
				parent = -1;
			else if (pickedShape % 3 == 1)
			{
				parent = pickedShape - 1;
				glm::vec4 pos = B->GetControlPoint(i, 0);
				dx = p.x - pos.x;//position relatively to the parent
				dy = p.y - pos.y;
			}
			else if (pickedShape % 3 == 2)
			{
				glm::vec4 pos;
				parent = pickedShape + 1;
				if (i < B->GetSegmentsNum() - 1)
					pos = B->GetControlPoint(i + 1, 0);
				else
					pos = B->GetControlPoint(i, 3);//if it's the last parent
				dx = p.x - pos.x;
				dy = p.y - pos.y;
			}
			shapes.insert(shapes.begin()+ind, new Shape(Octahedron, TRIANGLES));
			chainParents.insert(chainParents.begin()+ind, -1);
			AddShapeViewport(ind, 1);
			if (parent == -1)
			{
				ShapeTransformation(xTranslate, p.x*scale);
				ShapeTransformation(yTranslate, p.y*scale);
			}
			else
			{
				ShapeTransformation(xTranslate, dx*scale);
				ShapeTransformation(yTranslate, dy*scale );
			}
			shapes[ind]->MyScale(glm::vec3(0.15*scale, 0.15*scale, 0.15*scale));
			SetParent(pickedShape, parent);
			RemoveShapeViewport(pickedShape, 0);
			ind++;
		}
	pickedShape = -1;
}

void Bezier::Update(const glm::mat4 &proj, glm::mat4 &view,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	if (shapes[pickedShape]->GetMaterial() >= 0 && !materials.empty())
		BindMaterial(s, shapes[pickedShape]->GetMaterial());
	//textures[0]->Bind(0);
	s->Bind();
	if (shaderIndx < 3)
	{
		s->SetUniformMat4f("MVP", proj*view);
		s->SetUniformMat4f("Normal", Model);
	}
	else
	{
		s->SetUniformMat4f("proj", proj);
		s->SetUniformMat4f("view", view);
		s->SetUniformMat4f("Normal", Model);
	}
	//s->SetUniform1i("sampler1", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(0));
	//	s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	s->SetUniform4f("lightColor", r / 255.0f, 0.0f, 0.0f, 0.0f);
	s->Unbind();
}

void Bezier::WhenRotate()
{
}

void Bezier::WhenTranslate()
{
}

void Bezier::Motion()
{
	if(isActive)
	{
	}
}

unsigned int Bezier::TextureDesine(int width, int height)
{
	unsigned char* data = new unsigned char[width * height * 4];
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			data[(i * height + j) * 4] = (i + j) % 256;
			data[(i * height + j) * 4 + 1] = (i + j * 2) % 256;
			data[(i * height + j) * 4 + 2] = (i * 2 + j) % 256;
			data[(i * height + j) * 4 + 3] = (i * 3 + j) % 256;
		}
	}
	textures.push_back(new Texture(width, height));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //note GL_RED internal format, to save 
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] data;
	return(textures.size() - 1);
}

void Bezier::MouseProccessing(int button, int xrel, int yrel)
{
	if (picked != -1 && picked <= B->GetSegmentsNum() * 3 + 3)
	{
		if (button == 1)
		{
			pickedShape = picked;
			if ((!continuity) || (((pickedShape-3) % 3) == 0))
			{
				glm::vec4 p = B->GetControlPoint((pickedShape - 3) / 3, (pickedShape - 3) % 3);
				glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / 3, (chainParents[pickedShape] - 3) % 3);
				if (!(p.y + yrel/(350.0f*scale) < 0 && (pickedShape - 3) % 3 == 0))
				{
					ShapeTransformation(xTranslate, -xrel / 350.0f);
					ShapeTransformation(yTranslate, yrel / 350.0f);
					B->CurveUpdate(picked, -xrel / (350.0f*scale), yrel / (350.0f*scale), false);

					//calculate the angle between p and and it's parent x axis
					float r = sqrtf(powf(p.x - c.x, 2) + powf(p.y - c.y, 2));
					float l = sqrtf(powf(c.x + r - p.x, 2) + powf(c.y - p.y, 2));
					float angle = acos((2 * powf(r, 2) - powf(l, 2)) / (2 * powf(r, 2)));
					if (p.y < c.y)
						angle = -angle;
					B->updateAngle(picked, angle * 180 / pi);
				}
			}
			else
			{
				glm::vec4 p = B->GetControlPoint((pickedShape - 3) / 3, (pickedShape - 3) % 3);
				glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / 3, (chainParents[pickedShape] - 3) % 3);
				float m = (p.y - c.y) / (p.x - c.x);
				float n = p.y - p.x*m;
				float y = (p.x - xrel/350.0f)*m + n;
				float m2 = (y - c.y) / (p.x - xrel / 350.0f - c.x);
				
					if (p.x != c.x)
					{
						ShapeTransformation(xTranslate, -xrel / 350.0f);
						ShapeTransformation(yTranslate, y - p.y);
						B->CurveUpdate(picked, -xrel / (350.0f*scale), (p.x - xrel / (350.0f*scale))*m + n - p.y, false);
					}
					else//if the point is on a perp line with its parent
					{
						ShapeTransformation(yTranslate, yrel/350.0f);
						B->CurveUpdate(picked, 0, yrel/(350.0f*scale), false);
					}
				
			}
			pickedShape = -1;

		}
		else
		{
			pickedShape = picked;
			if ((pickedShape - 3) % 3 == 0)
				return;
			glm::vec4 p = B->GetControlPoint((pickedShape - 3) / 3, (pickedShape - 3) % 3);
			ShapeTransformation(zRotate, -yrel / 2.0f);
			int yrel2 = yrel;
			glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / 3, (chainParents[pickedShape] - 3) % 3);
			if (p.y < c.y)
				yrel = -yrel;
			float rad = (-yrel / 2.0f)*(pi / 180);
			float r = sqrtf(powf(p.x - c.x, 2) + powf(p.y - c.y, 2));
			float l = sqrtf(powf(c.x+r-p.x,2) + powf(c.y - p.y, 2));
			float angle = acos((2 * powf(r, 2) - powf(l, 2)) / (2 * powf(r, 2))) + (rad);
			if (p.y < c.y)
				angle = -angle;
			float dx = c.x + (r*cos(angle)) - p.x;
			float dy = c.y + (r*sin(angle)) - p.y;
			printf("dx:%f dy:%f angle:%f\n", dx, dy, angle*180/pi);
			B->CurveUpdate(picked,dx,dy, false);
			B->updateAngle(picked, angle * 180 / pi);

			if (continuity)
			{
				int seg = (pickedShape - 3) / 3;
				int indx = (pickedShape - 3) % 3;
				glm::vec4 bro;
				if (indx == 1  && seg > 0)//find the brother of the shape and move him accordingly
				{
					bro = B->GetControlPoint(seg-1, 2);
					pickedShape -= 2;
				}
				else if(indx == 2 && seg < B->GetSegmentsNum() - 1)
				{
					bro = B->GetControlPoint(seg + 1, 1);
					pickedShape += 2;
				}
				else
					return;
				processAngle(yrel2);//update the brother's ctrl pt angle
				//B->CurveUpdate(pickedShape, dx, dy, false);
			}

			pickedShape = -1;
		
			WhenRotate();
		}
	}

}

void Bezier::MouseProccessing3d(int button, int xrel, int yrel)
{
	if (picked != -1)
	{
		if (button == 1)
		{
			for (int i = 0; i < pickedShapes.size(); i++)
			{
				pickedShape = pickedShapes[i];
				if (pickedShape > B->GetSegmentsNum() * 3 + 4)
				{
					std::vector<glm::vec3> v = calcDirections();//calc the new right, up , and towards vector with the new cam angle
					glm::vec3 right = v[0];
					glm::vec3 up = v[1];
					glm::vec3 to = v[2];
					ShapeTransformation(xTranslate, -(xrel / 80.0f)*right.x);//moving according to the direction vectors
					ShapeTransformation(zTranslate, -(xrel / 80.0f)*right.z);

					ShapeTransformation(xTranslate, (yrel / 80.0f)*up.x);
					ShapeTransformation(yTranslate, (yrel / 80.0f)*up.y);
					ShapeTransformation(zTranslate, (yrel / 80.0f)*up.z);
					
					dynamic_cast<Bezier2D*>(shapes[pickedShape])->changePos(glm::vec3(-(xrel / 80.0f)*right.x + (yrel / 80.0f)*up.x,
						(yrel / 80.0f)*up.y, -(xrel / 80.0f)*right.z + (yrel / 80.0f)*up.z));
				}
			}
		}
		else
		{
			if (picked > B->GetSegmentsNum() * 3 + 4)
			{
				std::vector<glm::vec3> v = calcDirections();
				glm::vec3 right = v[0];
				glm::vec3 up = v[1];
				glm::vec3 to = v[2];
				pickedShape = picked;
				Bezier2D* M = dynamic_cast<Bezier2D*>(shapes[pickedShape]);
				ShapeTransformation(xTranslate, -M->getPos().x);//translate to the center
				ShapeTransformation(yTranslate, -M->getPos().y);
				ShapeTransformation(zTranslate, -M->getPos().z);
				//ShapeTransformation(yRotate, (-xrel / 2.0f)*right.x);
				//ShapeTransformation(xRotate, -xrel / 2.0f*right.z);
				shapes[pickedShape]->MyRotate((-xrel/2.0f), up, 0);
				shapes[pickedShape]->MyRotate((-yrel/2.0f), right, 0);

				//ShapeTransformation(xRotate, (-yrel / 2.0f)*up.y);
				//ShapeTransformation(zRotate, -yrel / 2.0f);
				//ShapeTransformation(zRotate, -yrel / 2.0f);
				ShapeTransformation(xTranslate, M->getPos().x);
				ShapeTransformation(yTranslate, M->getPos().y);
				ShapeTransformation(zTranslate, M->getPos().z);
			}
		}
	}
	
}



void Bezier::convexHull(int button, int xrel, int yrel,float xpos, float ypos)
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	xpos = -(2 * viewport[2] * 0.75 - xpos);
	ypos = -(viewport[3] /2 - (viewport[3]-ypos));
	if (picked == -1)
	{
		int seg = B->CheckconvexHull(xpos / (350.0f*scale), ypos / (350.0f*scale));//find the segment with these points on c.h.
		printf("picked convex hull %d\n", seg);
		//for (int i = 0; i < 1; i++)
		//{
		if (seg != -1)
		{
			if(button == 1)
			{
				picked = seg * 3 + 3;
				
				MouseProccessing(button, xrel, yrel);
				picked = seg * 3 + 6;
				MouseProccessing(button, xrel, yrel);
				picked = -1;
			}
			else if(B->GetSegmentsNum() < 6)
			{
				pickedShape = seg * 3 + 3 + 1;
				//gather shapes to their parent's positions
				glm::vec4 p = B->GetControlPoint(seg, 1);
				glm::vec4 c = B->GetControlPoint(seg, 0);
				ShapeTransformation(xTranslate, (c.x-p.x)*scale);
				ShapeTransformation(yTranslate, (c.y-p.y)*scale);

				 p = B->GetControlPoint(seg, 2);
				 c = B->GetControlPoint(seg, 3);
				pickedShape++;
				ShapeTransformation(xTranslate, (c.x - p.x)*scale);
				ShapeTransformation(yTranslate, (c.y - p.y)*scale);
				pickedShape++;
				ShapeTransformation(xTranslate, -c.x*scale);
				ShapeTransformation(yTranslate, -c.y*scale);

				B->SplitSegment(seg, 0.5);
				//move the shapes to the new 1st half of the segment
				pickedShape = seg * 3 + 3 + 1;
				p = B->GetControlPoint(seg, 1);
				c = B->GetControlPoint(seg, 0);
				ShapeTransformation(xTranslate, (p.x - c.x)*scale);
				ShapeTransformation(yTranslate, (p.y - c.y)*scale);
				pickedShape++;
				p = B->GetControlPoint(seg, 2);
				c = B->GetControlPoint(seg, 3);
				ShapeTransformation(xTranslate, (p.x - c.x)*scale);
				ShapeTransformation(yTranslate, (p.y - c.y)*scale);
				pickedShape++;
				ShapeTransformation(xTranslate, c.x*scale);
				ShapeTransformation(yTranslate, c.y*scale);
				//add new shapes to the 2nd half
				std::vector<Shape*>::iterator it;
				std::vector<int>::iterator it2;
				it = shapes.begin() + 3 + (seg+1)*3 + 1;
				it2 = chainParents.begin()+ (B->GetSegmentsNum() - 1) * 3 + 4;
				shapes.insert(it, new Shape(Octahedron, TRIANGLES));
				RemoveShapeViewport(3 + (seg + 1) * 3 + 1, 0);
				it = shapes.begin() + 3 + (seg + 1) * 3 + 2;
				shapes.insert(it, new Shape(Octahedron, TRIANGLES));
				RemoveShapeViewport(3 + (seg + 1) * 3 + 2, 0);
				it = shapes.begin() + 3 + (seg + 1) * 3 + 3;
				shapes.insert(it, new Shape(Octahedron, TRIANGLES));
				RemoveShapeViewport(3 + (seg + 1) * 3 + 3, 0);
				//add new parents to the list
				chainParents.insert(it2, (B->GetSegmentsNum() - 1) * 3 + 3);
				it2 = chainParents.begin() + (B->GetSegmentsNum() - 1) * 3 + 5;
				chainParents.insert(it2, (B->GetSegmentsNum() - 1) * 3 + 6);
				it2 = chainParents.begin() + (B->GetSegmentsNum() - 1) * 3 + 6;
				chainParents.insert(it2, -1);
				//chainParents.push_back((B->GetSegmentsNum() - 1) * 3 + 3);
				//chainParents.push_back((B->GetSegmentsNum() - 1) * 3 + 6);
				//chainParents.push_back(-1);

				//move new shpes to their positions
				pickedShape++;
				p = B->GetControlPoint(seg+1, 1);
				ShapeTransformation(xTranslate, (p.x - c.x)*scale);
				ShapeTransformation(yTranslate, (p.y - c.y)*scale);
				shapes[pickedShape]->MyScale(glm::vec3(0.15*scale, 0.15*scale, 0.15*scale));
				AddShapeViewport(pickedShape, 1);
				
				pickedShape++;
				p = B->GetControlPoint(seg+1, 2);
				c = B->GetControlPoint(seg+1, 3);
				ShapeTransformation(xTranslate, (p.x - c.x)*scale);
				ShapeTransformation(yTranslate, (p.y - c.y)*scale);
				shapes[pickedShape]->MyScale(glm::vec3(0.15*scale, 0.15*scale, 0.15*scale));
				AddShapeViewport(pickedShape, 1);

				
				pickedShape++;
				p = B->GetControlPoint(seg + 1, 3);
				ShapeTransformation(xTranslate, p.x*scale);
				ShapeTransformation(yTranslate, (p.y)*scale);
				shapes[pickedShape]->MyScale(glm::vec3(0.15*scale, 0.15*scale, 0.15*scale));
				AddShapeViewport(pickedShape, 1);

				

			}
		}
	}
	
}

void Bezier::processAngle(int yrel)
{
	ShapeTransformation(zRotate, -yrel / 2.0f);
	glm::vec4 p = B->GetControlPoint((pickedShape - 3) / 3, (pickedShape - 3) % 3);
	glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / 3, (chainParents[pickedShape] - 3) % 3);
	if (p.y < c.y)
		yrel = -yrel;
	float rad = (-yrel / 2.0f)*(pi / 180);
	float r = sqrtf(powf(p.x - c.x, 2) + powf(p.y - c.y, 2));
	float l = sqrtf(powf(c.x + r - p.x, 2) + powf(c.y - p.y, 2));
	float angle = acos((2 * powf(r, 2) - powf(l, 2)) / (2 * powf(r, 2))) + (rad);
	if (p.y < c.y)
		angle = -angle;
	float dx = c.x + (r*cos(angle)) - p.x;
	float dy = c.y + (r*sin(angle)) - p.y;
	printf("dx:%f dy:%f angle:%f\n", dx, dy, angle * 180 / pi);
	B->CurveUpdate(pickedShape, dx, dy, false);
	B->updateAngle(pickedShape, angle * 180 / pi);
}
void Bezier::straighten()
{
	pickedShape = picked;
	if ((pickedShape - 3) % 3 != 0)
		return;
	glm::vec4 pL = B->GetControlPoint((pickedShape - 3) / 3 -1, 2);//p2
	glm::vec4 pr = B->GetControlPoint((pickedShape - 3) / 3, 1);//p1
	glm::vec4 c = B->GetControlPoint((pickedShape - 3) / 3, (pickedShape - 3) % 3);//p0
	glm::vec4 np;
	float angle;
	float r1, r2;
	if (pL.z != -1 && pr.z !=-1)//if the points exist
	{
		r1 = sqrtf(powf(pL.x - c.x, 2) + powf(pL.y - c.y, 2));
		if(pr.z!=-1)
			r2 = sqrtf(powf(pr.x - c.x, 2) + powf(pr.y - c.y, 2));
		if (pL.w < 0)
			angle = pL.w + 180;
		else
			angle = pL.w - 180;
		float deg = angle * 180 / pi;
		picked = pickedShape + 1;
		MouseProccessing(0,0, ( -2*(angle- pr.w)));//rotate shape to be 180 degrees from parent
	}
	else if ((pL.z != -1))
	{
		picked = pickedShape - 1;
		MouseProccessing(0, 0, (-2 * (90-pL.w)));
	}
	else
	{
		picked = pickedShape +1;
		MouseProccessing(0, 0, (-2 * (90 - pr.w)));
	}
		
	  
}

void Bezier::reset(int num)
{
	shapes.erase(shapes.begin() + 3, shapes.begin() + B->GetSegmentsNum() * 3 + 4);
	B->reset(num);
	createShapes();
}
void Bezier::releaseR()
{
	picked = -1;
}

void Bezier::releaseL()
{
	picked = -1;
}

void Bezier::Continuity()
{
	continuity = !continuity;
}

void Bezier::zoom(int dir)
{
	float size;
	if (dir < 0)
		size = 0.95;
	if (dir > 0)
		size = 1.05;
	for (int i = 0; i < B->GetSegmentsNum() * 3 + 4; i++)
	{
		
		shapes[i]->MyScale(glm::vec3(size, size, size));
		if (i > 2)
		{
			pickedShape = i;
			glm::vec4 p = B->GetControlPoint((pickedShape - 3) / 3, (pickedShape - 3) % 3);
			glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / 3, (chainParents[pickedShape] - 3) % 3);
			ShapeTransformation(xTranslate, (c.x-p.x)*(1-size)*scale);
			ShapeTransformation(yTranslate, (c.y - p.y)*(1-size)*scale);
		}
	}
	scale = scale * size;
}

void Bezier::zoom3d(int dir)
{
	for (int i = 0; i < pickedShapes.size(); i++)
	{
		pickedShape = pickedShapes[i];
		if (pickedShape > B->GetSegmentsNum() * 3 + 4)
		{
			std::vector<glm::vec3> v = calcDirections();//calculate new direction vectors
			glm::vec3 to = v[2];
			ShapeTransformation(xTranslate, dir*to.x);
			ShapeTransformation(yTranslate, dir*to.y);
			ShapeTransformation(zTranslate, dir*to.z);
			dynamic_cast<Bezier2D*>(shapes[pickedShape])->changePos(glm::vec3(dir*to.x, dir*to.y, dir*to.z));
		}
	}
}

void Bezier::create3D()
{
	Bezier2D* B2 = new Bezier2D(B,4,20,20,QUADS);
	chainParents.push_back(-1);
	shapes.push_back(B2);
	AddShapeViewport(shapes.size()-1, 0);
	shapes[shapes.size() - 1]->MyScale(glm::vec3(0.5, 0.5, 0.5));
	SetShapeShader(shapes.size() - 1, 4);
	
	//RemoveShapeViewport(shapes.size()-1, 1);
}

std::vector<glm::vec3> Bezier::calcDirections()
{
	float angle = cameraAngle * pi / 180.0f;
	float anglex = cameraAnglex * pi / 180.0f;
	glm::mat3 roty{ cos(angle), 0 , -sin(angle),0,1,0,sin(angle), 0, cos(angle) };//rotation matrix on y axis
	glm::mat3 rotx{ 1, 0 , 0,0,cos(anglex),sin(anglex),0, -sin(anglex), cos(anglex) };// rotation matrix on x axis

	glm::vec3 right(1, 0, 0);
	float e = sin(pi / 2.0f);
	//glm::mat3 rotz { cos(pi/2), sin(pi / 2) , 0,-sin(pi / 2),cos(pi / 2),0,0, 0, 1 };
	right = roty * right;//rotate right vector to new angle
	right = glm::normalize(right);

	glm::vec3 to(0, 0, -1);
	to = rotx * to;//rotate the toward vector to new angles
	to = roty * to;
	to = glm::normalize(to);

	glm::vec3 up = glm::cross(right, to);
	up = glm::normalize(up);
	std::vector<glm::vec3> v;
	v.push_back(right);
	v.push_back(up);
	v.push_back(to);
	return v;
	

}

void Bezier::correctPickingList()//erase cubemap from picking list
{
	std::vector<int>::iterator it = std::find(pickedShapes.begin(), pickedShapes.end(), B->GetSegmentsNum() * 3 + 4);
	if((it !=pickedShapes.end()))
		pickedShapes.erase(it);
}


Bezier::~Bezier(void)
{

}
