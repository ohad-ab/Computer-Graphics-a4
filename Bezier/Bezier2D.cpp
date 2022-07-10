#include "Bezier2D.h"
#include "glm/gtc/matrix_transform.hpp"

const float pi = 2 * acos(0);

Bezier2D::Bezier2D(const Bezier1D* c, int circularSubdivision, int _resS, int _resT, int mode, int viewport)
{

	//mesh->Bind();
	this->mode = mode;
	isCopy = false;
	viewports = viewport;
	shaderID = 1;
	materialID = 0;
	this->resT = _resT;
	this->resS = _resS;
	this->circularSubdivision = circularSubdivision;
	const Bezier1D* B1 = c;
	CalcControlPoints(B1);
	segSNum = c->GetSegmentsNum();
	segTNum = 2;
	pos = glm::vec3(0, 0, 0);
	mesh = new MeshConstructor(GetSurface(), true);
}

Bezier2D::Bezier2D(void)
{

}

void Bezier2D::CalcControlPoints(const Bezier1D* c)
{
	for (int i = 0; i < c->GetSegmentsNum(); i++)
		CalcSegControlPoints(c,i);
	
	

}

void Bezier2D:: CalcSegControlPoints(const Bezier1D* c, int seg)
{
	//float g = (4 * (1 - cos(0.5*(pi / 2)))) / (3 * sin(0.5*(pi / 2)));

	//glm::vec4 p0 = c->GetControlPoint(seg, 0);
	//glm::vec4 p1 = c->GetControlPoint(seg, 1);
	//glm::vec4 p2 = c->GetControlPoint(seg, 2);
	//glm::vec4 p3 = c->GetControlPoint(seg, 3);

	//controlPoints.push_back(p0);
	//controlPoints.push_back(p1);
	//controlPoints.push_back(p2);
	//controlPoints.push_back(p3);

	//controlPoints.push_back(glm::vec4(p0.x, p0.y, p0.y*g, 0));
	//controlPoints.push_back(glm::vec4(p1.x, p1.y, p1.y*g, 0));
	//controlPoints.push_back(glm::vec4(p2.x, p2.y, p2.y*g, 0));
	//controlPoints.push_back(glm::vec4(p3.x, p3.y, p3.y*g, 0));

	//controlPoints.push_back(glm::vec4(p0.x, p0.y*g, p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, p1.y*g, p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, p2.y*g, p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, p3.y*g, p3.y, 0));

	//controlPoints.push_back(glm::vec4(p0.x, 0, p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, 0, p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, 0, p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, 0, p3.y, 0));
	//
	////2nd quarter
	//
	//controlPoints.push_back(glm::vec4(p0.x, 0, p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, 0, p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, 0, p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, 0, p3.y, 0));

	//controlPoints.push_back(glm::vec4(p0.x, -p0.y*g, p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, -p1.y*g, p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, -p2.y*g, p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, -p3.y*g, p3.y, 0));

	//controlPoints.push_back(glm::vec4(p0.x, -p0.y, p0.y*g, 0));
	//controlPoints.push_back(glm::vec4(p1.x, -p1.y, p1.y*g, 0));
	//controlPoints.push_back(glm::vec4(p2.x, -p2.y, p2.y*g, 0));
	//controlPoints.push_back(glm::vec4(p3.x, -p3.y, p3.y*g, 0));

	//controlPoints.push_back(glm::vec4(p0.x, -p0.y, 0, 0));
	//controlPoints.push_back(glm::vec4(p1.x, -p1.y, 0, 0));
	//controlPoints.push_back(glm::vec4(p2.x, -p2.y, 0, 0));
	//controlPoints.push_back(glm::vec4(p3.x, -p3.y, 0, 0));

	////3rd quarter

	//controlPoints.push_back(glm::vec4(p0.x, -p0.y, 0, 0));
	//controlPoints.push_back(glm::vec4(p1.x, -p1.y, 0, 0));
	//controlPoints.push_back(glm::vec4(p2.x, -p2.y, 0, 0));
	//controlPoints.push_back(glm::vec4(p3.x, -p3.y, 0, 0));

	//controlPoints.push_back(glm::vec4(p0.x, -p0.y, -p0.y*g, 0));
	//controlPoints.push_back(glm::vec4(p1.x, -p1.y, -p1.y*g, 0));
	//controlPoints.push_back(glm::vec4(p2.x, -p2.y, -p2.y*g, 0));
	//controlPoints.push_back(glm::vec4(p3.x, -p3.y, -p3.y*g, 0));

	//controlPoints.push_back(glm::vec4(p0.x, -p0.y*g, -p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, -p1.y*g, -p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, -p2.y*g, -p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, -p3.y*g, -p3.y, 0));

	//controlPoints.push_back(glm::vec4(p0.x, 0, -p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, 0, -p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, 0, -p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, 0, -p3.y, 0));

	////4th
	//controlPoints.push_back(glm::vec4(p0.x, 0, -p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, 0, -p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, 0, -p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, 0, -p3.y, 0));

	//controlPoints.push_back(glm::vec4(p0.x, p0.y*g, -p0.y, 0));
	//controlPoints.push_back(glm::vec4(p1.x, p1.y*g, -p1.y, 0));
	//controlPoints.push_back(glm::vec4(p2.x, p2.y*g, -p2.y, 0));
	//controlPoints.push_back(glm::vec4(p3.x, p3.y*g, -p3.y, 0));

	//controlPoints.push_back(glm::vec4(p0.x, p0.y, -p0.y*g, 0));
	//controlPoints.push_back(glm::vec4(p1.x, p1.y, -p1.y*g, 0));
	//controlPoints.push_back(glm::vec4(p2.x, p2.y, -p2.y*g, 0));
	//controlPoints.push_back(glm::vec4(p3.x, p3.y, -p3.y*g, 0));

	//controlPoints.push_back(p0);
	//controlPoints.push_back(p1);
	//controlPoints.push_back(p2);
	//controlPoints.push_back(p3);

	float g = (4 * (1 - cos(0.5*(pi )))) / (3 * sin(0.5*(pi )));

	glm::vec4 p0 = c->GetControlPoint(seg, 0);
	glm::vec4 p1 = c->GetControlPoint(seg, 1);
	glm::vec4 p2 = c->GetControlPoint(seg, 2);
	glm::vec4 p3 = c->GetControlPoint(seg, 3);

	controlPoints.push_back(p0);
	controlPoints.push_back(p1);
	controlPoints.push_back(p2);
	controlPoints.push_back(p3);

	controlPoints.push_back(glm::vec4(p0.x, p0.y, p0.y*g, 0));
	controlPoints.push_back(glm::vec4(p1.x, p1.y, p1.y*g, 0));
	controlPoints.push_back(glm::vec4(p2.x, p2.y, p2.y*g, 0));
	controlPoints.push_back(glm::vec4(p3.x, p3.y, p3.y*g, 0));

	controlPoints.push_back(glm::vec4(p0.x, -p0.y, p0.y*g, 0));
	controlPoints.push_back(glm::vec4(p1.x, -p1.y, p1.y*g, 0));
	controlPoints.push_back(glm::vec4(p2.x, -p2.y, p2.y*g, 0));
	controlPoints.push_back(glm::vec4(p3.x, -p3.y, p3.y*g, 0));

	controlPoints.push_back(glm::vec4(p0.x, -p0.y, 0, 0));
	controlPoints.push_back(glm::vec4(p1.x, -p1.y, 0, 0));
	controlPoints.push_back(glm::vec4(p2.x, -p2.y, 0, 0));
	controlPoints.push_back(glm::vec4(p3.x, -p3.y, 0, 0));


	//2nd half

	controlPoints.push_back(glm::vec4(p0.x, -p0.y, 0, 0));
	controlPoints.push_back(glm::vec4(p1.x, -p1.y, 0, 0));
	controlPoints.push_back(glm::vec4(p2.x, -p2.y, 0, 0));
	controlPoints.push_back(glm::vec4(p3.x, -p3.y, 0, 0));

	controlPoints.push_back(glm::vec4(p0.x, -p0.y, -p0.y*g, 0));
	controlPoints.push_back(glm::vec4(p1.x, -p1.y, -p1.y*g, 0));
	controlPoints.push_back(glm::vec4(p2.x, -p2.y, -p2.y*g, 0));
	controlPoints.push_back(glm::vec4(p3.x, -p3.y, -p3.y*g, 0));

	controlPoints.push_back(glm::vec4(p0.x, p0.y, -p0.y*g, 0));
	controlPoints.push_back(glm::vec4(p1.x, p1.y, -p1.y*g, 0));
	controlPoints.push_back(glm::vec4(p2.x, p2.y, -p2.y*g, 0));
	controlPoints.push_back(glm::vec4(p3.x, p3.y, -p3.y*g, 0));

	controlPoints.push_back(p0);
	controlPoints.push_back(p1);
	controlPoints.push_back(p2);
	controlPoints.push_back(p3);

}


glm::vec3 Bezier2D::GetNormal(int segmentS, int segmentT, float s, float t)
{
	return glm::vec3(0,0,0);
}

IndexedModel Bezier2D::GetSurface()
{
	IndexedModel model;
	int ind = 0;
	for (int k = 0; k < segSNum; k++)
	{
		int begPos = model.positions.size();
		for (int d = 0; d < segTNum; d++)
		{
			for (int j = 0; j <= resS; j++)
			{
				for (unsigned int i = 0; i <= resT; i++)
				{
					Vertex* v = GetPointOnSurface(k, d, (float)j / resS, (float)i / resT);
					glm::vec3 p = *v->GetPos();
					glm::vec2 tex = *v->GetTexCoord();
					glm::vec3 normal = -*v->GetNormal();
					model.positions.push_back(p);
					model.colors.push_back(glm::vec3(0, 1, 1));
					model.normals.push_back(normal);
					model.texCoords.push_back(tex);
				}
			}
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					//model.indices.push_back(ind + resS + 1);
					//model.indices.push_back(ind);
					//model.indices.push_back(ind + 1);

					//model.indices.push_back(ind + resS + 1);
					//model.indices.push_back(ind + 1);
					//model.indices.push_back(ind + resS + 2);
					model.indices.push_back(ind + resS + 1);
					model.indices.push_back(ind);
					model.indices.push_back(ind + 1);
					model.indices.push_back(ind + resS + 2);

					ind++;

				}
				ind++;
				
			}

			


			//ind++;

		}
		for (int j = begPos; j < begPos+20; j++)
		{
			//model.indices.push_back(j);
			//model.indices.push_back(ind);
			//model.indices.push_back(ind + 1);

			//model.indices.push_back(j);
			//model.indices.push_back(ind + 1);
			//model.indices.push_back(j +1);
			model.indices.push_back(j);
			model.indices.push_back(ind);
			model.indices.push_back(ind + 1);
			model.indices.push_back(j + 1);

			ind++;

		}
		ind += 22;
	}
	
	


	return model;
}




Vertex* Bezier2D::GetPointOnSurface(int segmentS, int segmentT, float s, float t)
{
	glm::vec4 b = glm::vec4(0, 0, 0, 0);
	glm::vec4 ds = glm::vec4(0, 0, 0, 0);
	glm::vec4 dt = glm::vec4(0, 0, 0, 0);
	
	for (int i = 0; i < 4; i++)
	{
		glm::vec4 yy = glm::vec4(0, 0, 0, 0);
		for (int j = 0; j < 4; j++)
		{
			//int bini = binomialCoeff(3, i);
			//int binj = binomialCoeff(3, j);
			//glm::vec4 p = controlPoints[32 * segmentS + 16 * segmentT + 4 * i + j];
			//float pows1 = powf(s, i - 1);
			//float pows2 = powf(1 - s, 3 - i);
			//float pows3 = -powf(s, i);
			//float pows4 = (3 - i)*powf(1 - s, 3 - i - 1);
			//float powt = powf(t, j);
			//float powt1 = powf(1 - t, 3 - j);
			//yy += powf(t, j)*powf(1 - t, 3 - j)*binomialCoeff(3, j)*controlPoints[32 * segmentS + 16 * segmentT + 4 * i + j];
			int f = i == 0 ? 0 : powf(s, i - 1);//if power is 0 then the derivative is 0
			int h = j == 0 ? 0 : powf(t, j - 1);
			int f1 = i == 3 ? 0 : powf(1 - s, 3 - i - 1);//if i == 3 then the power is 0 so the derivative is 0
			int h1 = j == 3 ? 0 : powf(1-t, 3 - j - 1);
			float ee = binomialCoeff(3, i)*(f*i*powf(1 - s, 3 - i) - powf(s, i)*(3 - i)*f1);

			glm::vec4 test = binomialCoeff(3, i)*powf(t, j)*powf(1 - t, 3 - j)*binomialCoeff(3, j)*controlPoints[32 * segmentS + 16 * segmentT + 4 * i + j] * (f*i*powf(1 - s, 3 - i) - powf(s, i)*(3 - i)*f1);
			b += binomialCoeff(3, i)*powf(s, i)*powf(1 - s, 3 - i)*binomialCoeff(3, j)*powf(t, j)*powf(1 - t, 3 - j)*controlPoints[32 * segmentS + 16 * segmentT + 4 * i + j];
			ds += binomialCoeff(3, i)*powf(t, j)*powf(1 - t, 3 - j)*binomialCoeff(3, j)*controlPoints[32 * segmentS + 16 * segmentT + 4 * i + j] * (f*i*powf(1 - s, 3 - i) - powf(s, i)*(3 - i)*f1);
			dt += binomialCoeff(3, i)*powf( s, i)*powf(1 - s, 3 - i)*binomialCoeff(3, j)*controlPoints[32 * segmentS + 16 * segmentT + 4 * i + j] * (h * j*powf(1-t, 3 - j) - powf(t, j)*(3 - j)*h1);
			if (ds == glm::vec4(0.f, 0.f, 0.f, 0.f) && dt == glm::vec4(0.f, 0.f, 0.f, 0.f))
				int g = 0;
		}
	}
	//glm::vec3 q(controlPoints[0].x, controlPoints[0].y, controlPoints[0].z);
	//glm::vec3 q1(controlPoints[1].x, controlPoints[1].y, controlPoints[1].z);
	//glm::vec3 q2(controlPoints[4].x, controlPoints[4].y, controlPoints[4].z);
	//glm::vec3 q3(controlPoints[5].x, controlPoints[5].y, controlPoints[5].z);
	//glm::vec3 q4 = -3.f*q + 3.f*q1;
	//glm::vec3 q5 = -3.f*q2 + 3.f*q3;
	//glm::vec3 w = glm::normalize(glm::cross(-3.f*q + 3.f*q1, -3.f*q2 + 3.f*q3));
	glm::vec3 normal(glm::cross(glm::vec3(ds.x, ds.y, ds.z), glm::vec3(dt.x, dt.y, dt.z)));
	if (normal != glm::vec3(0, 0, 0))
		normal = glm::normalize(normal);
	return &Vertex(glm::vec3(b.x, b.y, b.z), glm::vec2(0.5*b.x, 0.5*b.y), normal, glm::vec3(1, 0, 0));
}

int Bezier2D::binomialCoeff(int n, int k)
{
	// Base Cases
	if (k > n)
		return 0;
	if (k == 0 || k == n)
		return 1;

	// Recur
	return binomialCoeff(n - 1, k - 1)
		+ binomialCoeff(n - 1, k);
}

void Bezier2D::changePos(glm::vec3 delta)
{
	pos += delta;
}

glm::vec3 Bezier2D::getPos()
{
	return pos;
}

Bezier2D::~Bezier2D(void)
{

}