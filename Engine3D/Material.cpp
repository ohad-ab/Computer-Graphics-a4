#include "Material.h"


Material::Material(unsigned int textureIDs[], unsigned int size)
{
	numOfTexs = size;
	for (int i = 0; i < size; i++)
	{
		texIDs[i] = textureIDs[i];
	}
}

unsigned int Material::GetTexture(unsigned int indx)
{
	if (indx < numOfTexs)
		return texIDs[indx];
	return 0;
}


Material::~Material()
{
	
}


