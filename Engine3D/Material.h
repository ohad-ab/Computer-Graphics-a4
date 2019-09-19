#pragma once
#include "texture.h"

class Material
{
	//Should use smart pointers for two materials with the same texture.
	unsigned int numOfTexs;
	unsigned int texIDs[];

public:
	Material(unsigned int textureIDs[],unsigned int size);
	unsigned int GetTexture(unsigned int indx);


	~Material();
};

