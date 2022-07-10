#include "game.h"
#include <iostream>
#include "GL/glew.h"
#include "Bezier1D.h"
#include <math.h>

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
	
	AddTexture("../res/textures/box0.bmp",2);
	//AddTexture("../res/textures/grass.bmp", 2);
	//TextureDesine(800, 800);

	AddMaterial(texIDs,slots, 2);
	AddMaterial(texIDs+1,slots+1, 2);
	AddShape(Cube, -1, TRIANGLES);
	AddShape(Axis, -1, LINES);
	B = new Bezier1D(1, 20, LINE_STRIP);
	chainParents.push_back(-1);
	shapes.push_back(B);
	AddShapeViewport(2, 1);
	//AddShape(Plane, -1, TRIANGLES);
	AddShapeViewport(1, 1);
	RemoveShapeViewport(2, 0);
	RemoveShapeViewport(1, 0);
	//SetShapeShader(2, 2);

	pickedShape = 0;
	SetShapeMaterial(0, 0);
	ShapeTransformation(xTranslate, -1);

	pickedShape = 1;
	//shapes[pickedShape]->MyScale(glm::vec3(10,10,10));
	//AddShape(Octahedron, -1, TRIANGLES);
	//AddShapeViewport(3, 1);
	shapes[2]->MyScale(glm::vec3(0.2, 0.2, 0.2));
	//ShapeTransformation(yScale, 100);
	//ShapeTransformation(yScale, 10);
	//ShapeTransformation(zScale, 10);
	//SetShapeMaterial(1, 1);
	pickedShape = -1;
	int ind = 3;
	for(int i=0; i<B->GetSegmentsNum(); i++)
		for (int j = 0; j < 3 ||(j==3 && i== B->GetSegmentsNum()-1); j++)
		{
			int parent = -1;
			float dx = 0, dy = 0;
			pickedShape = ind;
			glm::vec4 p = B->GetControlPoint(i, j);
			if (pickedShape%3 == 0)
				parent = -1;
			else if (pickedShape % 3 == 1)
			{
				parent = pickedShape - 1;
				glm::vec4 pos = B->GetControlPoint(i, 0);
				dx = p.x- pos.x;
				dy = p.y - pos.y;
			}
			else if (pickedShape % 3 == 2)
			{
				glm::vec4 pos;
				parent = pickedShape + 1;
				if(i < B->GetSegmentsNum() - 1)
					pos = B->GetControlPoint(i+1, 0);
				else
					pos = B->GetControlPoint(i, 3);
				dx = p.x - pos.x;
				dy = p.y - pos.y;
			}
			
			
			AddShape(Octahedron, -1, TRIANGLES);
			AddShapeViewport(ind, 1);
			if (parent == -1)
			{
				ShapeTransformation(xTranslate, p.x*0.2);
				ShapeTransformation(yTranslate, p.y*0.2);
			}
			else
			{
				ShapeTransformation(xTranslate, dx*0.2);
				ShapeTransformation(yTranslate, dy*0.2);
			}
			shapes[ind]->MyScale(glm::vec3(0.03, 0.03, 0.03));
			SetParent(pickedShape, parent);
			ind++;
		}
	pickedShape = -1;
	//pickedShape = 2;
	//SetShapeMaterial(2, 2);
	//pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Bezier::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	if (shapes[pickedShape]->GetMaterial() >= 0 && !materials.empty())
		BindMaterial(s, shapes[pickedShape]->GetMaterial());
	//textures[0]->Bind(0);
	s->Bind();
	if (shaderIndx != 2)
	{
		s->SetUniformMat4f("MVP", MVP);
		s->SetUniformMat4f("Normal", Model);
	}
	else
	{
		s->SetUniformMat4f("MVP", glm::mat4(1));
		s->SetUniformMat4f("Normal", glm::mat4(1));
	}
	s->SetUniform1i("sampler1", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(0));
	if(shaderIndx!=2)
		s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
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
	if (picked != -1)
	{
		if (button == 1)
		{
			pickedShape = picked;
			if (!continuity)
			{
				ShapeTransformation(xTranslate, -xrel / 350.0f);
				ShapeTransformation(yTranslate, yrel / 350.0f);
				B->CurveUpdate(picked, -xrel / 70.0f, yrel / 70.0f, false);

				//shapes[2]->MyScale(glm::vec3(5, 5, 5));
				glm::vec4 p = B->GetControlPoint((pickedShape - 3) / B->GetSegmentsNum(), (pickedShape - 3) % B->GetSegmentsNum());
				glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / B->GetSegmentsNum(), (chainParents[pickedShape] - 3) % B->GetSegmentsNum());
				float r = sqrtf(powf(p.x - c.x, 2) + powf(p.y - c.y, 2));
				float l = sqrtf(powf(c.x + r - p.x, 2) + powf(c.y - p.y, 2));
				float angle = acos((2 * powf(r, 2) - powf(l, 2)) / (2 * powf(r, 2)));
				if (p.y < c.y)
					angle = -angle;
				B->updateAngle(picked, angle * 180 / pi);
			}
			else
			{
				glm::vec4 p = B->GetControlPoint((pickedShape - 3) / B->GetSegmentsNum(), (pickedShape - 3) % B->GetSegmentsNum());
				glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / B->GetSegmentsNum(), (chainParents[pickedShape] - 3) % B->GetSegmentsNum());
				float m = (p.y - c.y) / (p.x - c.x);
				float n = p.y - p.x*m;
				float y = (p.x - xrel/350.0f)*m + n;
				float m2 = (y - c.y) / (p.x - xrel / 350.0f - c.x);
				
					ShapeTransformation(xTranslate, -xrel / 350.0f);
					ShapeTransformation(yTranslate, y - p.y);
					B->CurveUpdate(picked, -xrel / 70.0f, (p.x - xrel / 70.0f)*m + n -p.y, false);
				
			}
			//shapes[2]->MyScale(glm::vec3(0.2, 0.2, 0.2));
			//MyTranslate(glm::vec3(-xrel / 80.0f, 0, 0), 0);
			//MyTranslate(glm::vec3(0, yrel / 80.0f, 0), 0);
			//WhenTranslate();
			pickedShape = -1;

		}
		else
		{
			pickedShape = picked;
			if ((pickedShape - 3) % B->GetSegmentsNum() == 0)
				return;
			glm::vec4 p = B->GetControlPoint((pickedShape - 3) / B->GetSegmentsNum(), (pickedShape - 3) % B->GetSegmentsNum());
			//ShapeTransformation(xTranslate, -p.x/5);
			//ShapeTransformation(yTranslate, -p.y/5);
			ShapeTransformation(zRotate, -yrel / 2.0f);
			int yrel2 = yrel;
			glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / B->GetSegmentsNum(), (chainParents[pickedShape] - 3) % B->GetSegmentsNum());
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
			//ShapeTransformation(xTranslate, p.x / 5);
			//ShapeTransformation(yTranslate, p.y / 5);
			if (continuity)
			{
				int seg = (pickedShape - 3) / B->GetSegmentsNum();
				int indx = (pickedShape - 3) % B->GetSegmentsNum();
				glm::vec4 bro;
				if (indx == 1  && seg > 0)
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
				processAngle(yrel2);
				//B->CurveUpdate(pickedShape, dx, dy, false);
			}

			pickedShape = -1;
			//MyRotate(-xrel / 2.0f, glm::vec3(0, 1, 0), 0);
			//MyRotate(-yrel / 2.0f, glm::vec3(1, 0, 0), 1);
			WhenRotate();
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
		int seg = B->CheckconvexHull(xpos / 70.0f, ypos / 70.0f);
		printf("picked convex hull %d\n", seg);
		//for (int i = 0; i < 1; i++)
		//{
		if (seg != -1)
		{
			if(button == 1)
			{
				picked = seg * 3 + 3;
				//if(seg == B->GetSegmentsNum()-1)
				//	if(i==3)
				MouseProccessing(button, xrel, yrel);
				picked = seg * 3 + 6;
				MouseProccessing(button, xrel, yrel);
				picked = -1;
			}
			else
			{
				B->SplitSegment(seg, 0.5);

			}
		}
	}
	
}

void Bezier::processAngle(int yrel)
{
	ShapeTransformation(zRotate, -yrel / 2.0f);
	glm::vec4 p = B->GetControlPoint((pickedShape - 3) / B->GetSegmentsNum(), (pickedShape - 3) % B->GetSegmentsNum());
	glm::vec4 c = B->GetControlPoint((chainParents[pickedShape] - 3) / B->GetSegmentsNum(), (chainParents[pickedShape] - 3) % B->GetSegmentsNum());
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
	if ((pickedShape - 3) % B->GetSegmentsNum() != 0)
		return;
	glm::vec4 pL = B->GetControlPoint((pickedShape - 3) / B->GetSegmentsNum()-1, 2);
	glm::vec4 pr = B->GetControlPoint((pickedShape - 3) / B->GetSegmentsNum(), 1);
	glm::vec4 c = B->GetControlPoint((pickedShape - 3) / B->GetSegmentsNum(), (pickedShape - 3) % B->GetSegmentsNum());
	glm::vec4 np;
	float angle;
	float r1, r2;
	if (pL.z != -1 && pr.z !=-1)
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
		MouseProccessing(0,0, ( -2*(angle- pr.w)));
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
Bezier::~Bezier(void)
{

}
