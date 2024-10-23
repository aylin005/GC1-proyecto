#include "GameObject.h"


GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef) {
	this->OpenGL = OpenGL;
	this->hwnd = hwnd;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	ShaderOfModel = 0;
}

GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, string nombre, const char* objPath, string texturePath, string normalMappingPath) {
	this->OpenGL = OpenGL;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	this->hwnd = hwnd;
	this->nombre = nombre;
	ObjectPath = objPath;
	TexturePath1 = texturePath;
	NormalMapPath = normalMappingPath;
	ShaderOfModel = 0;
}

GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, string nombre, const char* objPath, string texturePath) {
	this->OpenGL = OpenGL;
	this->hwnd = hwnd;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	this->nombre = nombre;
	ObjectPath = objPath;
	TexturePath1 = texturePath;
	ShaderOfModel = 0;
}

GameObject::~GameObject() {
}

float * GameObject::GetWorldMatrix() {
	return this->worldmatrix;
}

string GameObject:: getNombre()
{
	return this->nombre;
}

bool GameObject::SetShaderModel(Shader* ModelShader) {
	ShaderOfModel = ModelShader;
	return true;
}

bool GameObject::AddTexture(string texturePath) {
	try {;
	model->AddTexture(texturePath, true);
		return true;
	}
	catch (exception e) {
		printf("Error,&s", e);
		return false;
	}
}

bool GameObject::Initialize() {
	bool result = true;
	string resultDetail = "";

	worldmatrix = new float[16] { 0.0f };
	if (!OpenGL) {
		result = false;
		_RPT1(0, "Alert! GameObject OpenGL does not defined. \n", 0);
		return result;
	}
	else
		OpenGL->BuildIdentityMatrix(worldmatrix);
	model = new Model(OpenGL, hwnd, LoaderTexRef);
	model->Initialize(ObjectPath, TexturePath1);
	if (!result) {
		_RPT1(0, "Alert! GameObject Model does not initializdd. \n", 0);
		return result;
	}

	return result;
}

void GameObject::ChangeTexture(int From, int To) {
	model->ChangeTexture(From, To);
}

void GameObject::Render(float* viewMatrix,float* projectionMatrix) {
	
	if (model) {
		ShaderOfModel->SetShader();
		ShaderOfModel->PonMatriz4x4((char*)"worldMatrix", worldmatrix);
		ShaderOfModel->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
		ShaderOfModel->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);
		model->Render(ShaderOfModel);
	}
}

float * GameObject::MultiplyVectorMatrix(float*matrix, float*vector) {
	float* vr = new float[4] {0.0f};
	vr[3] = 1.0f;
	vr[0] = vector[0] * matrix[0] + matrix[12];//matrix[0] * vector[0] + matrix[1] * vector[1] + matrix[2] * vector[2] + matrix[3] * vector[3];
	vr[1] = vector[1] * matrix[5] + matrix[13];//matrix[4] * vector[0] + matrix[5] * vector[1] + matrix[6] * vector[2] + matrix[7] * vector[3];
	vr[2] = vector[2] * matrix[10] + matrix[14];//matrix[8] * vector[0] + matrix[9] * vector[1] + matrix[10] * vector[2] + matrix[11] * vector[3];
	vr[3] = 1.0f;//matrix[12] * vector[0] + matrix[13] * vector[1] + matrix[14] * vector[2] + matrix[15] * vector[3];
	return vr;
}

void GameObject::Shutdown() {
	if (worldmatrix) {
		delete worldmatrix;
		worldmatrix = 0;
	}
	if (model)
		model = NULL;
}

void GameObject::Transform(float scale[3], float rotation[3], float position[3])
{
	//Matrices auxiliares
	float* WorldMatrix = this->GetWorldMatrix();
	float* auxTranslationMatrix = new float[16];
	float* auxScaleMatrix = new float[16];
	float* auxRotationMatrix = new float[16];
	float* auxRotationXYZ[3]{};

	for (int i = 0; i < 3; i++) // cada elemento del array es otro array asi que lo inicializo de esta manera
	{
		auxRotationXYZ[i] = new float[16]; // cada elemento es una matriz de 16 elementos
	}

	//Aplico escala y traslacion a cada matriz
	OpenGL->MatrixScale(auxScaleMatrix, scale[0], scale[1], scale[2]);
	OpenGL->MatrixTranslation(auxTranslationMatrix, position[0], position[1], position[2]);

	for (int i = 0; i < 3; i++)
	{
		if (rotation[i] == 0) // si el angulo de rotacion en ese eje es 0 se hace matriz identidad para no tener problemas
		{
			OpenGL->BuildIdentityMatrix(auxRotationXYZ[i]);
		}
		else // si hay un angulo en ese eje entonces
		{
			if (i == 0) // angulo en x
			{
				OpenGL->MatrixRotationX(auxRotationXYZ[0], rotation[0]);
			}
			else if (i == 1) // angulo en y
			{
				OpenGL->MatrixRotationY(auxRotationXYZ[1], rotation[1]);
			}
			else if (i == 2) // angulo en z
			{
				OpenGL->MatrixRotationZ(auxRotationXYZ[2], rotation[2]);
			}
		}
	}
	// se multiplican las matrices y el rotation final se guarda en auxRotationMatrix
	OpenGL->MatrixMultiply(auxRotationMatrix, auxRotationXYZ[2], auxRotationXYZ[1]);
	OpenGL->MatrixMultiply(auxRotationMatrix, auxRotationMatrix, auxRotationXYZ[0]);

	// Se aplican todas las transformaciones en el worldMatrix
	OpenGL->MatrixMultiply(WorldMatrix, auxScaleMatrix, auxRotationMatrix);
	OpenGL->MatrixMultiply(WorldMatrix, WorldMatrix, auxTranslationMatrix);
}