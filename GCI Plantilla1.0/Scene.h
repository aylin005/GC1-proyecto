#ifndef _Scene_
#define _Scene_

#include "openglclass.h"
#include "cameraclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Land.h"
#include "inputclass.h"
#include "KeyCode.h"
#include "VectorRR.h"
#include "Model.h"
#include "Triangle.h"
#include "colorshaderclass.h"
#include "ShaderBoundingBox.h"
#include "GameObject.h"
#include "Shader.h"
#include "TextureShader.h"
#include "TextureLoaderForOpenGL.h"
#include "Dome.h"
#include "ShaderSkyDome.h"
#include "Billboard.h"
#include "ShaderBillboard.h"
#include "BoundingBoxRenderer.h"
#include "vector"
#include <string>
#include <algorithm>

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float SPEED_AXIS_Y = 0.05f;
const float SPEED_AXIS_X = 0.05f;
const float SPEED_CAMERA = 0.01f;
//const float InitialPosition[3] = { -55.3f, 11.0f, 132.0f }; //Inicio del recorrido
const float InitialPosition[3] = { 40.467304, 5.800000, -86.949074 }; //Enfrente del rio
const float InitialRotation[3] = { 0.0f, 90, 0.0f };


using std::vector;
using namespace std;

class Scene {
private:
	//Estructuras
	struct informacionDColision
	{
		float Position[3];
		float Address[3];
		float Speed[3];
	};
public:
	struct Vector {
		float X;
		float Y;
		float Z;
	};
private:
	// Variables de movimiento y rotacion del personaje
	Vector* DeltaPosition = new Vector();
	Vector* DeltaRotation = new Vector();
	Vector* LasDeltaPosition = new Vector();

	//Dependencias Externas
	HWND handlerWindow = NULL;

	//Motor Grafico
	OpenGLClass* OpenGL;

	//Elementos Primarios
	CameraClass* Camera;
	Land* Terreno;
	informacionDColision* prevColision;
	TriangleClass* Triangulo;
	Dome* Skydome;

	//Shaders
	LightShaderClass* LightShader;
	ColorShaderClass* ColorShader;
	ShaderBoundingBox* ShaderBounding;
	ShaderTexture* ShaderModel;
	ShaderTexture* ShaderWater;
	SkydomeShaderClass* ShaderSkydome;
	BillboardShaderClass* ShaderBillboard;
	ShaderBoundingBox* ShaderBox;

	//Lights
	LightClass* Light;

	//Input
	KeyBoard KeyCode;
	InputClass* input;
	float deltaTime;

	float waterTime = 0;
	TextureClass* LoaderTexture;

	std::vector<GameObject*> Collisions; //Vector que guarda todos los Modelos que tendran colisiones

	//Objetos
	GameObject* Object3d;
	GameObject* Object3d1;
	GameObject* Object3d2;
	GameObject* Hongo;
	GameObject* Arco;
	GameObject* Puente;
	GameObject* Cabin;
	GameObject* Honguito1;
	GameObject* Honguito2;
	GameObject* Honguito3;
	GameObject* SakuraTree;
	GameObject* Columpio;
	GameObject* Arbol;
	GameObject* Ventana;
	GameObject* Puerta;
	GameObject* Star;
	GameObject* Water;

	//Billboards
	Billboard* Tree;
	Billboard* Pine1;
	Billboard* Pine2;
	Billboard* Arbusto1;
	Billboard* Arbusto2;

	//Cajas
	BoundingBoxRenderer* box;
	BoundingBoxRenderer* Cabana1;
	BoundingBoxRenderer* Cabana2;
	BoundingBoxRenderer* Cabana3;
	BoundingBoxRenderer* Cabana4;
	BoundingBoxRenderer* Cabana5;
	BoundingBoxRenderer* Cabana6;
	BoundingBoxRenderer* esc1;
	BoundingBoxRenderer* esc2;
	BoundingBoxRenderer* esc3;
	BoundingBoxRenderer* esc4;
	BoundingBoxRenderer* esc5;
	BoundingBoxRenderer* esc6;
	BoundingBoxRenderer* esc7;
	BoundingBoxRenderer* esc8;
	BoundingBoxRenderer* valla1;
	BoundingBoxRenderer* valla2;
	BoundingBoxRenderer* puentebox;
	BoundingBoxRenderer* Sakura;
	BoundingBoxRenderer* Sakura1;
	BoundingBoxRenderer* Sakura2;
	BoundingBoxRenderer* SakuraC;
	BoundingBoxRenderer* Sakura3;
	BoundingBoxRenderer* CasaArbol;
	BoundingBoxRenderer* AreaCasa;
	BoundingBoxRenderer* AreaColumpio;
	BoundingBoxRenderer* AreaPuente;
	BoundingBoxRenderer* AreaArco;
	BoundingBoxRenderer* AreaCabin;
	BoundingBoxRenderer* AreaHongos;
	BoundingBoxRenderer* Lim1;
	BoundingBoxRenderer* Lim2;
	BoundingBoxRenderer* Lim3;
	BoundingBoxRenderer* Lim4;

	//Variables de Movimiento (Traslacion)
	float speedAxisX;
	float speedAxisY;
	float speed;

	//Variables de Rotacion
	float angulo;
	float scale;
	bool Reset;

	//Variables de representacion (Color)
	float color[3];

	//Banderas de habilitacion
	bool RenderObject3d = true;
	bool RenderBilboard = true;

	//animacion estrella
	float starRotation = 0.0;
	float starFloating = 0.0;

	//VEHICULO
	bool is3rdPerson = false;

public:
	Scene(OpenGLClass* OpenGLObject, HWND hwnd);
	~Scene();

	bool Initialize();
	// Funcion cuya informacion de teclado o gamepad NO se le transfiere
	bool Render();
	bool Update(InputClass* input, float deltaTime);
	bool ManageCommands();
	bool Shutdown();
	bool setModel(GameObject*& model, ShaderTexture* ShaderModel, OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath);
	void returnLastPosition();
protected:


};

#endif //_Scene_