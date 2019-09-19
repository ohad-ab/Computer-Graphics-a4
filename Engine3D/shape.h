#pragma once
#include "MovableGLM.h"
#include "VertexArray.hpp"
#include "shader.h"
#include "MeshConstructor.h"

class Shape : public MovableGLM
{
private:

	MeshConstructor *mesh;
	unsigned int materialID;
	int shaderID;
	bool isCopy;
	unsigned int mode;
	bool toRender;

public:

	Shape(const Shape& shape,unsigned int mode);

	Shape(const std::string& fileName,unsigned int mode);
	
	Shape(const int SimpleShapeType,unsigned int mode);

	void Draw( const std::vector<Shader*> shaders,bool isPicking);

	inline void Hide() {toRender = false;}

	inline void Unhide() {toRender = true;}

	inline bool Is2Render() {return toRender;}

	inline bool Is2D(){return mesh->Is2D();}

	inline void SetShader(const int id){ shaderID = id;}

	inline int GetShader(){return shaderID;}

	inline void SetMaterial(const unsigned int id) { materialID = id; }

	inline unsigned int GetMaterial() { return materialID; }
	
	virtual ~Shape(void);
};

