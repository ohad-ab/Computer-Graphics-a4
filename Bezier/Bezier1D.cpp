#include "Bezier1D.h"

Bezier1D::Bezier1D(int segNum, int res, int mode, int viewport)
{
	
	//mesh->Bind();
	this->mode = mode;
	isCopy = false;
	viewports = viewport;
	shaderID = 1;
	materialID = 0;
	this->resT = res;
	//float c = ((4 / 3) * (sqrt(2.f * 81.f) - 9)/-9);
	M = { -1,3,-3,1,
			3,-6,3,0,
			-3,3,0,0,
			1,0,0,0};
	M = glm::transpose(M);
	this->segments = { glm::transpose(glm::mat4{ -5, 0, 0, 0,
									-5, 1.65684, 0, 90,
									-3.65684, 3, 0, 180,
									-2,3,0,0}),
						 glm::transpose(glm::mat4{ -2, 3, 0, 0,
									-1, 3, 0, 0,
									1, 3, 0, 180,
									2,3,0,0}),
						
						glm::transpose(glm::mat4{ 2, 3, 0, 0,
									3.65684 , 3, 0, 0,
									5, 1.65684, 0, 90,
									5,0,0,0}) };

	mesh = new MeshConstructor(GetLine(), false);
}

IndexedModel Bezier1D::GetLine()
{
	IndexedModel model;

	
	int ind = 0;
	for (int j = 0; j < segments.size(); j++)
	{
		for (unsigned int i = 0; i <= resT; i++)
		{
			glm::vec4 pos = GetPointOnCurve(j, i);
			model.positions.push_back(glm::vec3(pos.x, pos.y, 0));
			model.colors.push_back(glm::vec3(1, 0, 0));
			model.indices.push_back(ind);
			ind++;
		}
	}
		

	return model;
}

glm::vec4 Bezier1D::GetPointOnCurve(int segment, int t)
{
	float x = (float)t / (float)resT;
	glm::vec4 p(powf(x, 3), powf(x, 2), x, 1);

	glm::vec4 res = p * M*segments[segment];
	return res;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
	if (segment >= 0 && segment < segments.size())
		return glm::vec4(segments[segment][0][indx], segments[segment][1][indx], 0, segments[segment][3][indx]);
	else if (segment == segments.size() && indx == 0)
		return glm::vec4(segments[segment-1][0][3], segments[segment-1][1][3], 0, segments[segment-1][3][3]);
	else
		return glm::vec4(0, 0, -1, 0);
}

void Bezier1D::CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1)
{
	int seg = (pointIndx - 3) / 3;
	int p = (pointIndx - 3) % 3;
	if (seg == segments.size())
	{
		seg -= 1;
		p = 3;
	}
	MoveControlPoint(seg, p, dx, dy, preserveC1);
	mesh->ChangeLine(GetLine());
}

void Bezier1D::updateAngle(int pointIndx, float angle)
{
	int seg = (pointIndx - 3) / 3;
	int p = (pointIndx - 3) % 3;
	if(seg<GetSegmentsNum())
		segments[seg][3][p] = angle;
	else
		segments[seg-1][3][3] = angle;
}
void Bezier1D::MoveControlPoint(int segment, int indx, float dx, float dy, bool preserveC1)
{
	segments[segment][0][indx] += dx;
	segments[segment][1][indx] += dy;
	if (segment > 0 && segment < segments.size())
	{
		if (indx == 0) 
		{
			segments[segment - 1][0][3] += dx;
			segments[segment - 1][1][3] += dy;
			segments[segment - 1][0][2] += dx;
			segments[segment - 1][1][2] += dy;
			segments[segment][0][1] += dx;
			segments[segment][1][1] += dy;
		}
		if (indx == 3)
		{
			segments[segment][0][2] += dx;
			segments[segment][1][2] += dy;
		}
	}
	else if (segment == 0 && indx == 0)
	{
		segments[segment][0][1] += dx;
		segments[segment][1][1] += dy;
	}
	

}

float sign(std::pair<float, float> pt, std::pair<float, float> p2, std::pair<float, float> p3)
{
	return (pt.first - p3.first) * (p2.second - p3.second) - (p2.first - p3.first) * (pt.second - p3.second);
}

int Bezier1D::CheckconvexHull(float x, float y)
{
	std::pair<float, float>pt(x, y);
	for (int i = 0; i < segments.size(); i++)
	{
		int maxX = 0;//ind of ctrl pt max x on seg
		int minX= 0;
		for (int j = 1; j < 4; j++)
		{
			if (segments[i][0][j] > segments[i][0][maxX])
				maxX = j;
		}
		for (int j = 1; j < 4; j++)
		{
			if (segments[i][0][j] < segments[i][0][minX])
				minX = j;
		}
		if (maxX != minX)
		{
			//line between maxX and minX
			float m = (segments[i][1][maxX] - segments[i][1][minX]) / (segments[i][0][maxX] - segments[i][0][minX]);
			float n = segments[i][1][maxX] - segments[i][0][maxX] *m;
			float d, maxd=0, far=-1, near=-1;
			for (int j = 0; j < 4; j++)
			{
				d = (m*segments[i][0][j] - segments[i][1][j] + n) / (sqrt(powf(m, 2) + 1));//distance between cntrl pt to line between maxX to minX
				if (abs(d) > maxd)
				{
					maxd = abs(d);
					if (far != -1)
						near = far;
					far = j;
				}
				else if (abs(d)>0)
				{
					near = j;
				}
			}
			if (far != -1 && maxd>0.2)//there is a point with d>0 from the line
			{
				std::pair<float, float>a(segments[i][0][maxX], segments[i][1][maxX]);
				std::pair<float, float>b(segments[i][0][minX], segments[i][1][minX]);
				std::pair<float, float>c(segments[i][0][far], segments[i][1][far]);

				if (near != -1)//there is another point  with far>d>0
				{
					std::pair<float,float>ptNear(segments[i][0][near], segments[i][1][near]);
					
					if (CheckTriangle(ptNear, a, b, c) && CheckTriangle(pt, a, b, c))//check if point is in the triangle of the other 3
					{
						return i;
					}
					else
					{//checking if the input point is in one of to triangle created by points
						if ((sign(ptNear, a, b)<0 && sign(c, a, b)<0) || (sign(ptNear, a, b) > 0 && sign(c, a, b) > 0))//if near and far on the same side
						{
							if (c.first > ptNear.first)
							{
								if (CheckTriangle(pt, ptNear, b, c) || CheckTriangle(pt, a, b, c))
									return i;
							}
							else if(CheckTriangle(pt, ptNear, a, c) || CheckTriangle(pt, a, b, c))
									return i;
						}
						else//near and far on opposite sides
						{
							if (CheckTriangle(pt, ptNear, a, b) || CheckTriangle(pt, a, b, c))
								return i;
						}
						
					}
				}

				else if (CheckTriangle(pt, a, b, c))
				{
					return i;
				}
			}
			else
			{//rectangle
				std::pair<float, float>a(segments[i][0][maxX], segments[i][1][maxX]);
				std::pair<float, float>b(segments[i][0][minX], segments[i][1][minX]);
				float angle = atan(-1/m);
				float delta = 0.5 / sin(angle);
				float leftLine, rightLine;
				if (m != 0)
				{
					float perpM = -1 / m;
					float leftN = b.second - perpM * b.first;
					float rightN = a.second - perpM * a.first;
					leftLine = (pt.second - leftN) / perpM;
					rightLine = (pt.second - rightN) / perpM;
				}
				else
				{
					leftLine = b.first;
					rightLine = a.first;
				}
				
				if (pt.second < m*pt.first + n + abs(delta) && pt.second > m*pt.first + n - abs(delta)
					&& pt.first > leftLine && pt.first < rightLine)
					return i;
			}

		}
	}
	return -1;
}



bool Bezier1D::CheckTriangle(std::pair<float,float> pt, std::pair<float, float> a, std::pair<float, float> b, std::pair<float, float> c)
{

		float d1, d2, d3;
		bool has_neg, has_pos;

		d1 = sign(pt, a, b);
		d2 = sign(pt, b, c);
		d3 = sign(pt, c, a);

		has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(has_neg && has_pos);
	
}

void Bezier1D::SplitSegment(int segment, int t)
{
	glm::vec4 p0(segments[segment][0][0], segments[segment][1][0], 0, 0);
	glm::vec4 p1(segments[segment][0][1], segments[segment][1][1], 0, 0);
	glm::vec4 p2(segments[segment][0][2], segments[segment][1][2], 0, 0);
	glm::vec4 p3(segments[segment][0][3], segments[segment][1][3], 0, 0);

	glm::vec4 L0 = p0;
	glm::vec4 r3 = p3;
	glm::vec4 L1(0.5f*(p0.x+ p1.x), 0.5f*(p0.y + p1.y), 0, 0);
	glm::vec4 r2 = 0.5f*(p2 + p3);
	glm::vec4 L2 = 0.5f*(L1 + 0.5f*(p1 + p2));
	glm::vec4 r1 = 0.5f*(r2 + 0.5f*(p1 + p2));
	glm::vec4 L3 = 0.5f*(L2 + r1);
	glm::vec4 r0 = 0.5f*(L2+r1);
	L1.w = calcAngle(L0, L1);
	L2.w = calcAngle(L3, L2);
	r1.w = calcAngle(r0, r1);
	r2.w = calcAngle(r3, r2);
	segments[segment] = glm::transpose(glm::mat4(L0,L1,L2,L3));
	
	std::vector<glm::mat4>::iterator it ;
	it = segments.begin()+ segment+1;
	segments.insert(it, glm::transpose(glm::mat4(r0, r1, r2, r3)));
	delete mesh;
	mesh = new MeshConstructor(GetLine(), false);

}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{

}

float Bezier1D::calcAngle(glm::vec4 c, glm::vec4 p)
{
	float pi = 2 * acos(0);
	float r = sqrtf(powf(p.x - c.x, 2) + powf(p.y - c.y, 2));
	float l = sqrtf(powf(c.x + r - p.x, 2) + powf(c.y - p.y, 2));
	float angle = acos((2 * powf(r, 2) - powf(l, 2)) / (2 * powf(r, 2)));
	if (p.y < c.y)
		angle = -angle;
	return angle*180/pi;
}

void Bezier1D::reset(int num)
{
	if (num < 2 || num>6)
		return;
	this->segments.clear();
	segments.push_back(glm::transpose(glm::mat4{ -5, 0, 0, 0,
									-5, 1.65684, 0, 90,
									-3.65684, 3, 0, 180,
									-2,3,0,0 }));
	switch (num)
	{
	case 2: this->segments.clear();
		segments.push_back(glm::transpose(glm::mat4{ -3, 0, 0, 0,
									-3, 1.65684, 0, 90,
									-1.65684, 3, 0, 180,
									0,3,0,0 }));
		segments.push_back(glm::transpose(glm::mat4{ 0, 3, 0, 0,
									1.65684 , 3, 0, 0,
									3, 1.65684, 0, 90,
									3,0,0,0 }));
		delete mesh;
		mesh = new MeshConstructor(GetLine(), false);
		return;
	case 3: segments.push_back(glm::transpose(glm::mat4{ -2, 3, 0, 0,
									-1, 3, 0, 0,
									1, 3, 0, 180,
									2,3,0,0 }));
		break;
	case 4: segments.push_back(glm::transpose(glm::mat4{ -2, 3, 0, 0,
									-1.5, 3, 0, 0,
									-0.5, 3, 0, 180,
									0,3,0,0 }));
		segments.push_back (glm::transpose(glm::mat4{0,3,0,0,
										0.5,3,0,0,
										1.5,3,0,0,
										2,3,0,0}));
		break;
	case 5: segments.push_back(glm::transpose(glm::mat4{ -2, 3, 0, 0,
									-1.75, 3, 0, 0,
									-1.25, 3, 0, 180,
									-1,3,0,0 }));
		segments.push_back(glm::transpose(glm::mat4{ -1,3,0,0,
										-0.5,3,0,0,
										0.5,3,0,0,
										1,3,0,0 }));
		segments.push_back(glm::transpose(glm::mat4{ 1,3,0,0,
										1.25,3,0,0,
										1.75,3,0,0,
										2,3,0,0 }));
		break;
	case 6: segments.push_back(glm::transpose(glm::mat4{ -2, 3, 0, 0,
									-1.75, 3, 0, 0,
									-1.25, 3, 0, 180,
									-1,3,0,0 }));
		segments.push_back(glm::transpose(glm::mat4{ -1,3,0,0,
										-0.75,3,0,0,
										-0.25,3,0,0,
										0,3,0,0 }));
		segments.push_back(glm::transpose(glm::mat4{ 0,3,0,0,
										0.25,3,0,0,
										0.75,3,0,0,
										1,3,0,0 }));
		segments.push_back(glm::transpose(glm::mat4{ 1,3,0,0,
										1.25,3,0,0,
										1.75,3,0,0,
										2,3,0,0 }));
		break;
	default:
		break;
	}
	segments.push_back(glm::transpose(glm::mat4{ 2, 3, 0, 0,
									3.65684 , 3, 0, 0,
									5, 1.65684, 0, 90,
									5,0,0,0 }));
	delete mesh;
	mesh = new MeshConstructor(GetLine(), false);
}
Bezier1D::~Bezier1D(void) {

}