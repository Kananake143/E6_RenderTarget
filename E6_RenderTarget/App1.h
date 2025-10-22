// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void firstPass();
	void secondPass();
	void finalPass();
	void gui();

private:
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	OrthoMesh* orthoMesh;
	LightShader* lightShader;
	TextureShader* textureShader;

	RenderTexture* renderTexture;
	RenderTexture* renderTexture1;
	Light* light;
};

#endif