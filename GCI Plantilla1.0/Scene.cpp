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

	ShaderBox = new ShaderBoundingBox(OpenGL, handlerWindow,
		"shaders/vertexcolor.vs", "shaders/vertexcolor.ps");
	setModel(Hongo, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Hongo/hongo.obj", "recursos/Hongo/Mushroom2Albedo.png");
	Collisions.push_back(Hongo);

	setModel(Arco, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Arco/arco.obj", "recursos/Arco/arch_BaseColor.png");

	setModel(Puente, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Puente2/Puente2.obj", "recursos/Puente2/TexturaPuente.png");

	setModel(SakuraTree, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/SakuraTree/Sakura.obj", "recursos/SakuraTree/Sakura_img.png");

	//setModel(Columpio, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		//"recursos/Columpio/ColumpioS.obj", "recursos/SakuraTree/Sakura_img.png");

	setModel(Cabin, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/Cabin2/cabin2.obj", "recursos/Cabin2/WoodTexture.jpeg");

	setModel(Honguito1, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos2/Hongito1/hongito1.obj", "recursos/hongitos/Hongito1/Mushroom_1_BaseColor.png");
	Collisions.push_back(Honguito1);

	setModel(Honguito2, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos2/Hongito2/hongito2.obj", "recursos/hongitos/Hongito2/Mushroom_2_BaseColor.png");
	Collisions.push_back(Honguito2);

	setModel(Honguito3, ShaderModel, OpenGL, handlerWindow, LoaderTexture,
		"recursos/hongitos2/Hongito3/hongito3.obj", "recursos/hongitos/Hongito3/Mushroom_3_BaseColor.png");
	Collisions.push_back(Honguito3);

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

	Arco->Render(viewMatrix, projectionMatrix, 3.8, 2.7, 10.9, 2.5, 2.5, 2.5);
	Puente->Render(viewMatrix, projectionMatrix, -22.0f, 3.2, -97.0f, 2, 2, 2.5);
	Cabin->Render(viewMatrix, projectionMatrix, -50.7f, 3.2, -130.0f, 2.5, 2.5, 2.5, 0, 3.1416, 0);
	SakuraTree->Render(viewMatrix, projectionMatrix, 18.0f, 2.7, 90.0f, 2.5, 2.5, 2.5);
	SakuraTree->Render(viewMatrix, projectionMatrix, 40.0f, 2.7, 81.0f, 2.5, 2.5, 2.5);
	SakuraTree->Render(viewMatrix, projectionMatrix, 46.0f, 2.7, 53.0f, 2.5, 2.5, 2.5);
	//Columpio->Render(viewMatrix, projectionMatrix, 26.0f, 2.7, 66.0f, 2.5, 2.5, 2.5);

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

	//Colision
	for (int i = 0; i < Collisions.size(); i++)
	{
		if (Collisions[i]->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
			returnLastPosition();
	}

	//if (Cabin->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
	//{
	//	DeltaPosition->X = LasDeltaPosition->X;
	//	DeltaPosition->Y = LasDeltaPosition->Y;
	//	DeltaPosition->Z = LasDeltaPosition->Z;
	//}
	//if (box->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
	//{
	//	DeltaPosition->X = LasDeltaPosition->X;
	//	DeltaPosition->Y = LasDeltaPosition->Y;
	//	DeltaPosition->Z = LasDeltaPosition->Z;
	//}

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

void Scene::returnLastPosition()
{
	DeltaPosition->X = LasDeltaPosition->X;
	DeltaPosition->Y = LasDeltaPosition->Y;
	DeltaPosition->Z = LasDeltaPosition->Z;
}