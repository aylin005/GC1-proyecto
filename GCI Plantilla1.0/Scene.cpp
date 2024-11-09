#include "Scene.h"

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


	ShaderBox = new ShaderBoundingBox(OpenGL, handlerWindow,
		"shaders/vertexcolor.vs", "shaders/vertexcolor.ps");
	setModel(Hongo, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Hongo/hongo.obj", "recursos/Hongo/Mushroom2Albedo.png");

	setModel(Arco, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Arco/arco.obj", "recursos/Arco/arch_BaseColor.png");

	setModel(Puente, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Puente2/Puente2.obj", "recursos/Puente2/TexturaPuente.png");

	setModel(Cabin, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Cabin2/cabin2.obj", "recursos/Cabin2/WoodTexture.jpeg");

	setModel(Honguito1, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos/Hongito1/hongito1.obj", "recursos/hongitos/Hongito1/Mushroom_1_BaseColor.png");

	setModel(Honguito2, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos/Hongito2/hongito2.obj", "recursos/hongitos/Hongito2/Mushroom_2_BaseColor.png");

	setModel(Honguito3, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos/Hongito3/hongito3.obj", "recursos/hongitos/Hongito3/Mushroom_3_BaseColor.png");

	//Skydome
	ShaderSkydome = new SkydomeShaderClass(OpenGL, handlerWindow, "shaders/SkydomeShader.vs",
		"shaders/SkydomeShader.ps");
	Skydome = new Dome("recursos/Texturas/midday.png", OpenGL, LoaderTexture, 400);
	Skydome->Initialize();
	Skydome->SetShader(ShaderSkydome);
	Skydome->SetIterpolationDay(false, true);

	//Billboards
	ShaderBillboard = new BillboardShaderClass(OpenGL, handlerWindow, "shaders/billboard.vs",
		"shaders/billboard.ps");

	Tree = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/tree.png");
	Tree->Initialize(10.0f,5.0f);
	Tree->SetShader(ShaderBillboard);

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
	Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
	Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);
	// Genera la matriz de vista a partir de la posicion de la camara.
	Camera->Render();

	// Obtiene las matrices de vista, mundo y proyeccion de opengl y el objeto de la camara.
	OpenGL->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	OpenGL->GetProjectionMatrix(projectionMatrix);

	Skydome->CicloInterpolaciones();
	Skydome->Render(viewMatrix, projectionMatrix);

	// Renderizamos terreno
	Terreno->Render(OpenGL, worldMatrix, viewMatrix, projectionMatrix);

	// Renderizamos Triangulo (ya se logra ver en escena)
	//Triangulo->Render(viewMatrix, projectionMatrix);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Renderizado de modelos
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// se carga position, scale y rotation, scale y rotation tienen valores por default en caso de no haber cambios necesarios
	Hongo->Render(viewMatrix, projectionMatrix, 1.22, 5.8, -124.96,  2,2,2);
	Hongo->Render(viewMatrix, projectionMatrix, 29.285208, 5.800000, -109.304688,  1,1,1);
	Arco->Render(viewMatrix, projectionMatrix,  3.8, 2.7, 10.9,      2.5, 2.5, 2.5);
	Puente->Render(viewMatrix, projectionMatrix, -22.0f, 3.2, -97.0f,     2, 2, 2.5);
	//Cabin->Render(viewMatrix, projectionMatrix,  -30.0f, 3.2, -130.0f,  2.5, 2.5, 2.5,   0, 3.1416, 0);
	Cabin->setTransform(-50.7f, 3.2, -130.0f, 2.5, 2.5, 2.5, 0, 3.1416, 0);
	Cabin->Render(viewMatrix, projectionMatrix, true);
	Honguito1->Render(viewMatrix, projectionMatrix, -40.19, 3.8, -125.77,  1,1,1,  0,3.14,0);
	Honguito1->Render(viewMatrix, projectionMatrix, 39, 5.800000, -117.748398);
	Honguito2->Render(viewMatrix, projectionMatrix, -41.19, 3.8, -126.77,  1,1,1,  0,3.14,0);
	Honguito2->Render(viewMatrix, projectionMatrix, 24.0f, 3.8, 16.0f, 2,2,2);
	Honguito3->Render(viewMatrix, projectionMatrix, 22.0f, 3.8, 17.0f);
	Honguito3->Render(viewMatrix, projectionMatrix, 25.0f, 3.8, 17.0f);

	//Billboards
	Tree->Render(viewMatrix, projectionMatrix,
		-15.0f, Terreno->Superficie(0.0f, 0.0f), 5.0f,
		DeltaPosition->X, DeltaPosition->Z);

	//Cajas
	box->Draw(viewMatrix, projectionMatrix);

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

	if (Cabin->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
	{
		DeltaPosition->X = LasDeltaPosition->X;
		DeltaPosition->Y = LasDeltaPosition->Y;
		DeltaPosition->Z = LasDeltaPosition->Z;
	}
	if (box->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
	{
		DeltaPosition->X = LasDeltaPosition->X;
		DeltaPosition->Y = LasDeltaPosition->Y;
		DeltaPosition->Z = LasDeltaPosition->Z;
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Transformaciones de modelos
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	return result;
}

bool Scene::ManageCommands() {
	bool result = true;

	LasDeltaPosition->X = DeltaPosition->X;
	LasDeltaPosition->Y = DeltaPosition->Y;
	LasDeltaPosition->Z = DeltaPosition->Z;

	float vr[2]{ 0,0 };
	float radians = DeltaRotation->Y * 0.0174532925f;

	if((input->GetKey(KeyCode.W) || input->GetKey(KeyCode.Up))) {
		vr[0] += sinf(radians)*speed * deltaTime;
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

	DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2;

	DeltaPosition->X += vr[0];
	DeltaPosition->Z += vr[1];

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