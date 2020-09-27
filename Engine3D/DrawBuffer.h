#pragma once
class DrawBuffer
{

	unsigned int buffer;
	unsigned int depthBuffer;
	unsigned int stencilBuffer;

	void CreateDepthBufferAttachment(int width, int height);
	void CreateColorBufferAttachment(int width, int height);
	void CreateStencilBufferAttachment(int width, int height);
public:
	enum
	{
		COLOR,
		DEPTH,
		STENCIL,
		BACK,
		FRONT,
		NONE,
	};
	DrawBuffer(int width, int height , bool stencil);
	void SetDrawDestination(int num, int mode);
	void resize(int width, int height);
	void Bind();
	void UnBind();
	~DrawBuffer(void);
};
