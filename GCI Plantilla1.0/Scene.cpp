#include "Scene.h"
#include <algorithm>

bool messageShown = false;
bool messageShown2 = false;
bool messageShown3 = false;
bool messageShown4 = false;
bool messageShown5 = false;
bool messageShown6 = false;

template<typename T>
const T& clamp(const T& value, const T& low, const T& high) {
	if (value < low) return low;
	if (value > high) return high;
	return value;
}

Scene::Scene(OpenGLClass* OpenGLObject, HWND hwnd) {
	handlerWindow = hwnd;
	OpenGL = OpenGLObject;
	Camera = 0;
	LightShader = 0;
	ColorShader = 0;
	ShaderBounding = 0;
	Triangulo = 0;
	Light = 0;
	speedAxisX = 0.0f;
	speedAxisY = 0.0f;
	speed = 0.0f;
	angulo = 0.0f;
	Object3d = 0;
	LoaderTexture = new TextureClass(OpenGL);
}

Scene::~Scene() {

}

bool Scene::Initialize() {
	bool result = true;
	string resultDetail = "";
	angulo = 0.0f;
	speedAxisX = SPEED_AXIS_X;
	speedAxisY = SPEED_AXIS_Y;
	speed = SPEED_CAMERA;

	// Crea un objeto camara.
	Camera = new CameraClass;
	if (!Camera) {
		result = false;
		_RPT1(0, "Alert! Camera has not been initialized. \n", 0);
		return result;
	}
	else {
		// Asigna las coordenadas de posicion y rotacion por defecto a los arreglos delta.
		DeltaPosition->X = InitialPosition[0]; DeltaPosition->Y = InitialPosition[1]; DeltaPosition->Z = InitialPosition[2];
		DeltaRotation->X = InitialRotation[0]; DeltaRotation->Y = InitialRotation[1]; DeltaRotation->Z = InitialRotation[2];
		// Luego se los pasamos a la camara.
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
		Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);
		// Con esto podriamos aislar siempre el punto de partida inicial de la escena para evitar teletrasportarse sin querer.
	}

	LightShader = new LightShaderClass(OpenGL, handlerWindow, "shaders/terrain.vs", "shaders/terrain.ps");
	if (!LightShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ColorShader = new ColorShaderClass(OpenGL, handlerWindow, "shaders/color.vs", "shaders/color.ps");
	if (!ColorShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the color shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! ColorShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	//Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/heightmap.jpg", "recursos/test.tga", "recursos/Piedras_normal.jpg",(float)400, (float)400, 0, 1);
	Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, 
		L"recursos/Texturas/Heightmap3.png", "recursos/Texturas/Rocks007.jpg", "recursos/Texturas/Grass003.jpg", "recursos/Texturas/MultiTexture.png",
		(float)600, (float)600);
	if (!Terreno){
		result = false;
		_RPT1(0, "Alert! Land has not been initialized. \n", 0);
		return result;
	}
	else {
		Terreno->SetLandShader(LightShader);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Carga de Modelos
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// Shader para los modelos
	ShaderModel = new ShaderTexture(OpenGL, handlerWindow, "shaders/light.vs", "shaders/light.ps");
	if (!ShaderModel) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ShaderBox = new ShaderBoundingBox(OpenGL, handlerWindow,
		"shaders/vertexcolor.vs", "shaders/vertexcolor.ps");
	//Hice esta clase setModel para que ocupe menos espacio, es lo mismo que se tenía antes pero en una función

	box = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f,10.0f ,10.0f });
	box->SetShader(ShaderBox);

	Cabana1 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f,5.0f ,1.0f });
	Cabana1->SetShader(ShaderBox);

	Cabana2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 6.0f,5.0f ,1.0f });
	Cabana2->SetShader(ShaderBox);

	Cabana3 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 6.0f,5.0f ,3.0f });
	Cabana3->SetShader(ShaderBox);

	Cabana4 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 1.0f,5.0f ,9.0f });
	Cabana4->SetShader(ShaderBox);

	Cabana5 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 2.0f,5.0f ,9.0f });
	Cabana5->SetShader(ShaderBox);

	Cabana6 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 1.0f,5.0f ,3.0f });
	Cabana6->SetShader(ShaderBox);

	puentebox = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 12.0f,5.0f ,30.0f });
	puentebox->SetShader(ShaderBox);

	valla1 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 1.0f,3.0f ,30.0f });
	valla1->SetShader(ShaderBox);

	valla2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 1.0f,3.0f ,30.0f });
	valla2->SetShader(ShaderBox);

	esc1 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 0.5f ,10.0f });
	esc1->SetShader(ShaderBox);

	esc2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 1.5f ,10.0f });
	esc2->SetShader(ShaderBox);

	esc3 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 2.5f ,10.0f });
	esc3->SetShader(ShaderBox);

	esc4 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 3.5f ,10.0f });
	esc4->SetShader(ShaderBox);

	esc5 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 0.5f ,10.0f });
	esc5->SetShader(ShaderBox);

	esc6 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 1.5f ,10.0f });
	esc6->SetShader(ShaderBox);

	esc7 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 2.5f ,10.0f });
	esc7->SetShader(ShaderBox);

	esc8 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 10.0f, 3.5f ,10.0f });
	esc8->SetShader(ShaderBox);

	Sakura = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 4.0f,3.0f ,4.0f });
	Sakura->SetShader(ShaderBox);

	Sakura1 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 4.0f,3.0f ,4.0f });
	Sakura1->SetShader(ShaderBox);

	Sakura2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 4.0f,3.0f ,4.0f });
	Sakura2->SetShader(ShaderBox);

	Sakura3 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 4.0f,3.0f ,4.0f });
	Sakura3->SetShader(ShaderBox);

	SakuraC = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 2.0f,4.0f ,2.0f });
	SakuraC->SetShader(ShaderBox);

	CasaArbol = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 5.0f,3.0f ,5.0f });
	CasaArbol->SetShader(ShaderBox);

	AreaCasa = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 20.0f,20.0f ,20.0f });
	AreaCasa->SetShader(ShaderBox);

	AreaColumpio = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 20.0f,20.0f ,20.0f });
	AreaColumpio->SetShader(ShaderBox);

	AreaPuente = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 20.0f,20.0f ,20.0f });
	AreaPuente->SetShader(ShaderBox);

	AreaArco = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 20.0f,20.0f ,10.0f });
	AreaArco->SetShader(ShaderBox);

	AreaHongos = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 30.0f,10.0f ,30.0f });
	AreaHongos->SetShader(ShaderBox);

	AreaCabin = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.0f,0.0f ,0.0f }, BoundingBox::GLFLOAT3{ 20.0f,10.0f ,20.0f });
	AreaCabin->SetShader(ShaderBox);

	ShaderBox = new ShaderBoundingBox(OpenGL, handlerWindow,
		"shaders/vertexcolor.vs", "shaders/vertexcolor.ps");

	setModel(Star, ShaderModel, OpenGL, handlerWindow, LoaderTexture, 
		"recursos/Star/objStar.obj", "recursos/Star/StarTextura.png");

	setModel(Hongo, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Hongo/hongo.obj", "recursos/Hongo/Mushroom2Albedo.png");
	Collisions.push_back(Hongo);

	setModel(Arco, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Arco/arco.obj", "recursos/Arco/arch_BaseColor.png");

	setModel(Puente, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Puente2/Puente2.obj", "recursos/Puente2/TexturaPuente.png");

	setModel(SakuraTree, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/SakuraTree/Sakura.obj", "recursos/SakuraTree/Sakura_img2.png");

	setModel(Columpio, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Columpio/Columpio2.obj", "recursos/Columpio/Sak_col2.png");

	setModel(Arbol, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Arbol/ArbolCasa.obj", "recursos/Arbol/ArbolCasa.png");

	setModel(Ventana, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Ventana/Ventana2.obj", "recursos/Ventana/window_BaseColor.png");

	setModel(Cabin, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Cabin2/cabin2.obj", "recursos/Cabin2/WoodTexture.jpeg");

	setModel(Honguito1, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos2/Hongito1/hongito1.obj", "recursos/hongitos2/Hongito1/Mushroom_1_BaseColor.png");
	Collisions.push_back(Honguito1);

	setModel(Honguito2, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos2/Hongito2/hongito2.obj", "recursos/hongitos2/Hongito2/Mushroom_2_BaseColor.png");
	Collisions.push_back(Honguito2);

	setModel(Honguito3, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos2/Hongito3/hongito3.obj", "recursos/hongitos2/Hongito3/Mushroom_3_BaseColor.png");
	Collisions.push_back(Honguito3);

	//Skydome
	ShaderSkydome = new SkydomeShaderClass(OpenGL, handlerWindow, "shaders/SkydomeShader.vs",
		"shaders/SkydomeShader.ps");
	Skydome = new Dome("recursos/Texturas/morning.png", OpenGL, LoaderTexture, 400);
	Skydome->Initialize();
	Skydome->SetShader(ShaderSkydome);
	Skydome->SetIterpolationDay(false, true);

	//Billboards
	ShaderBillboard = new BillboardShaderClass(OpenGL, handlerWindow, "shaders/billboard.vs",
		"shaders/billboard.ps");

	Pine1 = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Billboards/pine1.png");
	Pine1->Initialize(4.0f, 6.0f);
	Pine1->SetShader(ShaderBillboard);

	Pine2 = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Billboards/pine2.png");
	Pine2->Initialize(5.0f, 7.0f);
	Pine2->SetShader(ShaderBillboard);

	return result;
}

bool Scene::Render() {
	bool result = true;

	// Limpia los buffers para iniciar la escena nueva.
	OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Definimos nuestro espacio para las 3 matrices necesarias para renderizar.
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];

	// Actualiza la posicion y rotacion de la camara
	if (is3rdPerson) {
		float radius = 18.0f; // Distancia de la cámara al vehículo
		float offsetY = 7.0f; // Altura de la cámara sobre el vehículo

		float radiansYaw = DeltaRotation->Y * 0.0174532925f;
		float radiansPitch = DeltaRotation->X * 0.0174532925f;

		// Calcula la posición en coordenadas esféricas
		float camX = DeltaPosition->X - radius * cosf(radiansPitch) * sinf(radiansYaw);
		float camY = DeltaPosition->Y + offsetY - radius * sinf(radiansPitch);
		float camZ = DeltaPosition->Z - radius * cosf(radiansPitch) * cosf(radiansYaw);

		Camera->SetPosition(camX, camY, camZ);
		Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, 0.0f);
		//// Configura la cámara en tercera persona
		//Camera->SetPosition(DeltaPosition->X + 10, DeltaPosition->Y + 5, DeltaPosition->Z);
		//Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, 0.0f);
	}
	else {
		// Configura la cámara en primera persona (tu código actual)
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
		Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);
	}
	
	// Genera la matriz de vista a partir de la posicion de la camara.
	Camera->Render();

	// Obtiene las matrices de vista, mundo y proyeccion de opengl y el objeto de la camara.
	OpenGL->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	OpenGL->GetProjectionMatrix(projectionMatrix);

	//Skydome->CicloInterpolaciones();
	Skydome->Render(viewMatrix, projectionMatrix);

	// Renderizamos terreno
	Terreno->Render(OpenGL, worldMatrix, viewMatrix, projectionMatrix);

	// Renderizamos Triangulo (ya se logra ver en escena)
	//Triangulo->Render(viewMatrix, projectionMatrix);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Renderizado de modelos
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// se carga position, scale y rotation, scale y rotation tienen valores por default en caso de no haber cambios necesarios

	//Area de hongos gigantes
	Hongo->RenderWithCB(viewMatrix, projectionMatrix, true, 1.22, 5.8, -124.96,  2,2,2);
	if (Hongo->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	Hongo->RenderWithCB(viewMatrix, projectionMatrix, true, 29.285208, 5.8, -109.304688,  1,1,1);
	if (Hongo->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	Honguito1->RenderWithCB(viewMatrix, projectionMatrix, true, 9.64, Terreno->Superficie(9.64, -108), -108,  2, 2, 2,  0,3.14,0);
	if (Honguito1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	Honguito3->RenderWithCB(viewMatrix, projectionMatrix, true, 14.77, Terreno->Superficie(14.77, -94.64), -94.64, 2.5,2.5,2.5,   0,3.14,0);
	if (Honguito3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	Hongo->RenderWithCB(viewMatrix, projectionMatrix, true, 34.14, 5.8, -159.03, 1.5, 1.5, 1.5);
	if (Hongo->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	
	Honguito1->RenderWithCB(viewMatrix, projectionMatrix, true,-40.19, 3.8, -125.77,  1,1,1,  0,3.14,0);
	if (Honguito1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	Honguito2->RenderWithCB(viewMatrix, projectionMatrix, true, -41.19, 3.8, -126.77,  1,1,1,  0,3.14,0);
	if (Honguito2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();
	
	Honguito2->RenderWithCB(viewMatrix, projectionMatrix, true, -4.30, 3.8, 56.7, 2,2,2);
	if (Honguito2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	Honguito3->RenderWithCB(viewMatrix, projectionMatrix, true, -3.92, 3.8, -33.5f);
	if (Honguito3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	Honguito3->RenderWithCB(viewMatrix, projectionMatrix, true, 10.46f, 3.8, -8.41f);
	if (Honguito3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	//ESTRELLA
	if (starRotation <= 2 * 3.1416)
		starRotation += 0.02;
	else
		starRotation = 0.0;
	//if(starFloating)
	if (!is3rdPerson) {
		Star->RenderWithCB(viewMatrix, projectionMatrix, true, 25, 4, -20, 2, 2, 2, 0, starRotation, 0);
	}
	else {
		Star->RenderWithCB(viewMatrix, projectionMatrix, true, DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z, 2, 2, 2, 0, starRotation, 0);
	}
	Arco->Render(viewMatrix, projectionMatrix, 3.8, 2.7, 10.9, 2.5, 2.5, 2.5);
	Puente->Render(viewMatrix, projectionMatrix, -22.0f, 3.2, -97.0f, 2, 2, 2.5);
	Cabin->Render(viewMatrix, projectionMatrix, -50.7f, 3.2, -130.0f, 2.5, 2.5, 2.5, 0, 3.1416, 0);
	SakuraTree->Render(viewMatrix, projectionMatrix, 18.0f, 2.7, 90.0f, 2.5, 2.5, 2.5);
	SakuraTree->Render(viewMatrix, projectionMatrix, 40.0f, 2.7, 81.0f, 2.5, 2.5, 2.5);
	SakuraTree->Render(viewMatrix, projectionMatrix, 46.0f, 2.7, 53.0f, 2.5, 2.5, 2.5);
	Columpio->Render(viewMatrix, projectionMatrix, 26.0f, 2.7, 66.0f, 2.5, 2.5, 2.5);
	Arbol->Render(viewMatrix, projectionMatrix, -24.0f, 3.5, 35.0f, 3.5, 3.5, 3.5);
	Ventana->Render(viewMatrix, projectionMatrix, -22.2f, 4.5, 35.9f, 1.5, 1.5, 1.5, 0, 2.5708, -0.15);
	Ventana->Render(viewMatrix, projectionMatrix, -24.8f, 4.5, 33.3f, 1.5, 1.5, 1.5, 0, 1.8708, 0.15);

	//Billboards
	Pine1->Render(viewMatrix, projectionMatrix, -40.19f, Terreno->Superficie(-40.19f, -50.91f), -50.91f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -55.91f, Terreno->Superficie(-55.91f, -48.7f), -48.7f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -66.94f, Terreno->Superficie(-66.94f, -54.6f), -54.6f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 31.14f, Terreno->Superficie(31.14f, -44.94f), -44.94f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 54.4f, Terreno->Superficie(54.4f, -36.4f), -36.4f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 84.46f, Terreno->Superficie(84.46f, -49.49f), -49.49f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 90.31f, Terreno->Superficie(90.31f, -89.8f), -89.8f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 76.8f, Terreno->Superficie(76.8f, -104.7f), -104.7f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 44.53f, Terreno->Superficie(44.53f, -89.3f), -89.3f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -14.89f, Terreno->Superficie(-14.89f, -84.12f), -84.12f, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -33.57f, Terreno->Superficie(-33.57f, -93.94f), -93.94f, DeltaPosition->X, DeltaPosition->Z);


	Pine2->Render(viewMatrix, projectionMatrix, 207.5, Terreno->Superficie(207.5, -61.19), -61.19, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 223.36, Terreno->Superficie(223.36, -21.5), -21.5, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, 190.99, Terreno->Superficie(190.99, 6.82), 6.82, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, 200.25, Terreno->Superficie(200.25, 36.94), 36.94, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, 199.73, Terreno->Superficie(199.73, 81.46), 81.46, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, 187, Terreno->Superficie(187, 121.9), 121.9, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, 160.68, Terreno->Superficie(160.68, 156.15), 156.15, DeltaPosition->X, DeltaPosition->Z);
	

	Pine1->Render(viewMatrix, projectionMatrix, 2.78, Terreno->Superficie(2.78, 197.5), 197.5, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, -83.66, Terreno->Superficie(-83.66, 195.98), 195.98, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -135.65, Terreno->Superficie(-135.65, 175.7), 175.7, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -171.7, Terreno->Superficie(-171.7, 179.02), 179.02, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, -192.8, Terreno->Superficie(-192.8, 149.46), 149.46, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -192.89, Terreno->Superficie(-192.89, 149.46), 149.46, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -193.6, Terreno->Superficie(-193.6, 95.49), 95.49, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, -171.49, Terreno->Superficie(-171.49, 55.88), 55.88, DeltaPosition->X, DeltaPosition->Z);
	
	//Arboles de la cabaña
	Pine1->Render(viewMatrix, projectionMatrix, -67.54, Terreno->Superficie(-67.54, -115), -115, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -32.89, Terreno->Superficie(-32.89, -116.5), -116.5, DeltaPosition->X, DeltaPosition->Z);
	Pine2->Render(viewMatrix, projectionMatrix, -36.47, Terreno->Superficie(-36.47, -135.2), -135.2, DeltaPosition->X, DeltaPosition->Z);
	Pine1->Render(viewMatrix, projectionMatrix, -61.13, Terreno->Superficie(-61.13, -124.9), -124.9, DeltaPosition->X, DeltaPosition->Z);

	//Cajas
	box->Draw(viewMatrix, projectionMatrix);
	Cabana1->Draw(viewMatrix, projectionMatrix);
	Cabana2->Draw(viewMatrix, projectionMatrix);
	Cabana3->Draw(viewMatrix, projectionMatrix);
	Cabana4->Draw(viewMatrix, projectionMatrix);
	Cabana5->Draw(viewMatrix, projectionMatrix);
	Cabana6->Draw(viewMatrix, projectionMatrix);
	puentebox->Draw(viewMatrix, projectionMatrix);
	valla1->Draw(viewMatrix, projectionMatrix);
	valla2->Draw(viewMatrix, projectionMatrix);
	esc1->Draw(viewMatrix, projectionMatrix);
	esc2->Draw(viewMatrix, projectionMatrix);
	esc3->Draw(viewMatrix, projectionMatrix);
	esc4->Draw(viewMatrix, projectionMatrix);
	esc5->Draw(viewMatrix, projectionMatrix);
	esc6->Draw(viewMatrix, projectionMatrix);
	esc7->Draw(viewMatrix, projectionMatrix);
	esc8->Draw(viewMatrix, projectionMatrix);
	Sakura->Draw(viewMatrix, projectionMatrix);
	Sakura1->Draw(viewMatrix, projectionMatrix);
	Sakura2->Draw(viewMatrix, projectionMatrix);
	Sakura3->Draw(viewMatrix, projectionMatrix);
	SakuraC->Draw(viewMatrix, projectionMatrix);
	CasaArbol->Draw(viewMatrix, projectionMatrix);
	AreaCasa->Draw(viewMatrix, projectionMatrix);
	AreaColumpio->Draw(viewMatrix, projectionMatrix);
	AreaPuente->Draw(viewMatrix, projectionMatrix);
	AreaArco->Draw(viewMatrix, projectionMatrix);
	AreaHongos->Draw(viewMatrix, projectionMatrix);
	AreaCabin->Draw(viewMatrix, projectionMatrix);
	

	// Damos la instruccion de que termino la escena para que nos muestre frame a frame.
	OpenGL->EndScene();

	return result;
}

bool Scene::Update(InputClass* input, float deltaTime) {
	bool result = true;

	this->input = input;
	this->deltaTime = deltaTime;
	result = ManageCommands();
	if (!result) {
		result = false;
		_RPT1(0, "Alert! Error on key. \n", 0);
		return result;
	}

	float* matrixBox = box->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox, -10.0, Terreno->Superficie(-10.0f, 10.0f), 10.0f);

	float* matrixCabana = Cabana1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCabana, -54.0, Terreno->Superficie(-10.0f, 10.0f), -140.0f);

	float* matrixCabana2 = Cabana2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCabana2, -50.0, Terreno->Superficie(-10.0f, 10.0f), -132.0f);

	float* matrixCabana3 = Cabana3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCabana3, -51.0, Terreno->Superficie(-10.0f, 10.0f), -139.0f);

	float* matrixCabana4 = Cabana4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCabana4, -45.0, Terreno->Superficie(-10.0f, 10.0f), -140.0f);

	float* matrixCabana5 = Cabana5->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCabana5, -55.0, Terreno->Superficie(-10.0f, 10.0f), -140.0f);

	float* matrixCabana6 = Cabana6->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCabana6, -53.5, Terreno->Superficie(-10.0f, 10.0f), -131.0f);

	float* matrixPuente = puentebox->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixPuente, -2.0f, Terreno->Superficie(-10.0f, 10.0f), -80.0f);

	float* matrixvalla1 = valla1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixvalla1, 8.5f, 8.5f, -80.0f);

	float* matrixvalla2 = valla2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixvalla2, -2.3f, 8.5f, -80.0f);

	float* matrixesc1 = esc1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc1, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -54.0f);

	float* matrixesc2 = esc2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc2, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -55.5f);

	float* matrixesc3 = esc3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc3, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -57.0f);

	float* matrixesc4 = esc4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc4, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -59.0f);

	float* matrixesc5 = esc5->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc5, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -87.0f);

	float* matrixesc6 = esc6->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc6, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -85.5f);

	float* matrixesc7 = esc7->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc7, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -84.0f);

	float* matrixesc8 = esc8->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixesc8, -1.5f, Terreno->Superficie(-10.0f, 10.0f), -82.0f);

	float* matrixSakura = Sakura->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixSakura, 15.5f, Terreno->Superficie(-10.0f, 10.0f), 86.5f);

	float* matrixSakura1 = Sakura1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixSakura1, 37.5f, Terreno->Superficie(-10.0f, 10.0f), 77.5f);

	float* matrixSakura2 = Sakura2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixSakura2, 43.5f, Terreno->Superficie(-10.0f, 10.0f), 49.5f);

	float* matrixSakura3 = Sakura3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixSakura3, 24.0f, Terreno->Superficie(-10.0f, 10.0f), 64.0f);

	float* matrixSakuraC = SakuraC->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixSakuraC, 21.6f, Terreno->Superficie(-10.0f, 10.0f), 65.6f);

	float* matrixCasa = CasaArbol->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCasa, -26.4f, Terreno->Superficie(-10.0f, 10.0f), 32.5f);

	float* matrixAreaCasa = AreaCasa->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixAreaCasa, -31.4f, Terreno->Superficie(-10.0f, 10.0f), 24.5f);

	float* matrixAreaColumpio = AreaColumpio->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixAreaColumpio, 17.4f, Terreno->Superficie(-10.0f, 10.0f), 55.5f);

	float* matrixAreaPuente = AreaPuente->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixAreaPuente, -6.4f, Terreno->Superficie(-10.0f, 10.0f), -60.5f);

	float* matrixAreaArco = AreaArco->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixAreaArco, -6.4f, Terreno->Superficie(-10.0f, 10.0f), 7.5f);

	float* matrixAreaHongos = AreaHongos->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixAreaHongos, -2.4f, Terreno->Superficie(-10.0f, 10.0f), -130.5f);

	float* matrixAreaCabin = AreaCabin->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixAreaCabin, -58.4f, Terreno->Superficie(-10.0f, 10.0f), -145.5f);

	//Colision
	for (int i = 0; i < Collisions.size(); i++)
	{
		if (Collisions[i]->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
			returnLastPosition();
	}

	if (Cabana1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Cabana2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Cabana3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Cabana4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Cabana5->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Cabana6->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Sakura->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Sakura1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Sakura2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (Sakura3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (SakuraC->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (CasaArbol->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
		returnLastPosition();

	if (AreaCasa->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z) && !messageShown)
	{
		MessageBox(NULL, L"¡Está en la Casa del Arbol!", L"Casa del Arbol", MB_OK | MB_ICONEXCLAMATION);
		messageShown = true;
	}

	if (AreaColumpio->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z) && !messageShown2)
	{
		MessageBox(NULL, L"¡Está en el columpio!", L"Bosque de Sakura", MB_OK | MB_ICONEXCLAMATION);
		messageShown2 = true;
	}

	if (AreaPuente->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z) && !messageShown3)
	{
		MessageBox(NULL, L"¡Está en el puente!", L"Puente del río", MB_OK | MB_ICONEXCLAMATION);
		messageShown3 = true;
	}

	if (AreaArco->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z) && !messageShown4)
	{
		MessageBox(NULL, L"¡Está en el arco!", L"Arco de piedra", MB_OK | MB_ICONEXCLAMATION);
		messageShown4 = true;
	}

	if (AreaHongos->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z) && !messageShown5)
	{
		MessageBox(NULL, L"¡Está en el area de los hongos!", L"Bioma de Hongos", MB_OK | MB_ICONEXCLAMATION);
		messageShown5 = true;
	}

	if (AreaCabin->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z) && !messageShown6)
	{
		MessageBox(NULL, L"¡Se encuentra en la cabaña!", L"La Cabaña", MB_OK | MB_ICONEXCLAMATION);
		messageShown6 = true;
	}

	if (Star->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
	{
		//DeltaPosition->Y += 3;
		//Llamar función 3era persona
		is3rdPerson = true;
	}
		

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Transformaciones de modelos
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	return result;
}

// Variables para la posición de la estrella y la cámara
float starX = 0.0f, starY = 0.0f; // Posición de la estrella en el plano
float cameraRadius = 10.0f; // Radio de la órbita de la cámara
float cameraAngleX = 0.0f;  // Ángulo de rotación en el plano horizontal (yaw)
float cameraAngleY = 0.0f;  // Ángulo de rotación en el plano vertical (pitch)
const float minPitch = -1.5f, maxPitch = 1.5f; // Límites para el pitch

// Velocidades
const float moveSpeed = 0.6f;       // Velocidad de movimiento de la estrella
const float rotationSpeed = 0.6f; // Velocidad de rotación de la cámara

bool Scene::ManageCommands() {
	bool result = true;

	LasDeltaPosition->X = DeltaPosition->X;
	LasDeltaPosition->Y = DeltaPosition->Y;
	LasDeltaPosition->Z = DeltaPosition->Z;

	if (input->GetKeyDown(KeyCode.Backspace)) {
		is3rdPerson = !is3rdPerson; // Cambiar entre primera y tercera persona
	}

	//if (is3rdPerson) {
	//	// Movimiento de la estrella con w, a, s, d
	//	if (input->GetKey(KeyCode.A)) Star->setPosition(Star->getPositionX(), Star->getPositionY() + moveSpeed, Star->getPositionZ());
	//	if (input->GetKey(KeyCode.D)) Star->setPosition(Star->getPositionX(), Star->getPositionY() - moveSpeed, Star->getPositionZ());
	//	if (input->GetKey(KeyCode.W)) Star->setPosition(Star->getPositionX() - moveSpeed, Star->getPositionY(), Star->getPositionZ());
	//	if (input->GetKey(KeyCode.S)) Star->setPosition(Star->getPositionX() + moveSpeed, Star->getPositionY(), Star->getPositionZ());
	//}
	//else {
		float vr[2]{ 0,0 };
		float radians = DeltaRotation->Y * 0.0174532925f;

		if ((input->GetKey(KeyCode.W) || input->GetKey(KeyCode.Up))) {
			vr[0] += sinf(radians) * speed * deltaTime;
			vr[1] += cosf(radians) * speed * deltaTime;
		}
		if ((input->GetKey(KeyCode.S) || input->GetKey(KeyCode.Down))) {
			vr[0] -= sinf(radians) * speed * deltaTime;
			vr[1] -= cosf(radians) * speed * deltaTime;
		}
		if ((input->GetKey(KeyCode.A) || input->GetKey(KeyCode.Left))) {
			vr[0] -= sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
			vr[1] -= cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		}
		if ((input->GetKey(KeyCode.D) || input->GetKey(KeyCode.Right))) {
			vr[0] += sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
			vr[1] += cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		}

		if (input->GetRightAxisY() || input->GetRightAxisX()) {
			float angleX = input->GetRightAxisY() * speedAxisX * deltaTime;
			float angleY = input->GetRightAxisX() * speedAxisY * deltaTime;
			DeltaRotation->X -= angleX;
			DeltaRotation->Y += angleY;
		}

		if (is3rdPerson)
		{
			if (input->GetKey(KeyCode.Space))
				DeltaPosition->Y += speed * deltaTime;
			if(input->GetKey(KeyCode.Shift))
				DeltaPosition->Y -= speed * deltaTime;
		}

		if (input->GetKey(KeyCode.I)) {
			DeltaRotation->X -= speedAxisX * deltaTime;
		}
		if (input->GetKey(KeyCode.K)) {
			DeltaRotation->X += speedAxisX * deltaTime;
		}
		if (input->GetKey(KeyCode.J)) {
			DeltaRotation->Y -= speedAxisY * deltaTime;
		}
		if (input->GetKey(KeyCode.L)) {
			DeltaRotation->Y += speedAxisY * deltaTime;
		}

		if (input->GetKey(KeyCode.P))
		{
			string str = "Pos: " + to_string(DeltaPosition->X) + ", " + to_string(DeltaPosition->Y) + ", " + to_string(DeltaPosition->Z) + "\n";
			OutputDebugStringA(str.c_str());
		}

		if(!is3rdPerson)
			DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2;

		DeltaPosition->X += vr[0];
		DeltaPosition->Z += vr[1];

		// Limitar el rango del pitch (rotación vertical)
		if (DeltaRotation->X > 89.0f) {
			DeltaRotation->X = 89.0f;
		}
		if (DeltaRotation->X < -89.0f) {
			DeltaRotation->X = -89.0f;
		}
		
		//Que el vehículo no atraviese el terreno
		if (DeltaPosition->Y < Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2) {
			DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2; // Límite inferior.
		}
	return result;
}

bool Scene::Shutdown() {
	bool result = true;

	if (Camera)
	{
		delete Camera;
		Camera = 0;
	}

	if (LightShader)
	{
		LightShader->Shutdown();
		delete LightShader;
		LightShader = 0;
	}

	if (Triangulo) {
		Triangulo->Shutdown();
		delete Triangulo;
		Triangulo = 0;
	}

	if (Object3d) {
		Object3d->Shutdown();
		delete Object3d;
		Object3d = 0;
	}

	return result;
}

bool Scene::setModel(GameObject*& model,ShaderTexture* ShaderModel, OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath)
{
	bool result;
	model = new GameObject(OpenGL, hwnd, LoaderTextureRef,
		objPath, texturePath);
	if (!model) {
		result = false;
		MessageBoxA(hwnd, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = model->Initialize();
		if (!result) {
			MessageBoxA(hwnd, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		model->SetShaders(ShaderModel, ShaderBox);
	}
}

void Scene::returnLastPosition()
{
	DeltaPosition->X = LasDeltaPosition->X;
	DeltaPosition->Y = LasDeltaPosition->Y;
	DeltaPosition->Z = LasDeltaPosition->Z;
}

