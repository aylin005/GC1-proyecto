#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#include "openglclass.h"
#include "ShaderBoundingBox.h"
#include "lightshaderclass.h"
#include "Model.h"
#include "TextureLoaderForOpenGL.h"

class GameObject {
private:
	OpenGLClass* OpenGL;
	TextureClass* LoaderTexRef;
	Model* model;
	HWND hwnd;
	const char* ObjectPath;
	string TexturePath1;
	string TexturePath2;
	string TexturePath3;
	string TexturePath4;
	string NormalMapPath;
	float * worldmatrix;
	Shader* ShaderOfModel;
	Shader* ShaderOfBox;

	float center[4];
	BoundingBox::GLFLOAT3 points[8];
public:
	float x, y, z, sx, sy, sz, rx, ry, rz;
	GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef);
	GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath, string normalMappingPath);
	GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath);
	~GameObject();
	bool Initialize();
	void setTransform(float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
	bool SetShaderModel(Shader* ModelShader);
	bool SetShaders(Shader* ModelShader, Shader* BoxShader);
	bool AddTexture(string TextureAditional);
	void ChangeTexture(int From, int To);
	float* GetWorldMatrix();
	void Render(float* viewMatrix, float* projectionMatrix, bool showBox);
	void Render(float* viewMatrix, float* projectionMatrix, float posX, float posY, float posZ, float sx = 1, float sy = 1, float sz = 1, float rx = 0, float ry = 0, float rz = 0);
	void Transform();
	float* MultiplyVectorMatrix(float*,float*);
	float scale;
	void Shutdown();

	float* GetCenter();
	void UpdateBox();
	bool GetSphericalCollision(float, float, float, float);

	bool GetBoxCollision(float, float, float);
	float Dot(float*, float*);
	float Dot(float*, BoundingBox::GLFLOAT3);
};

#endif