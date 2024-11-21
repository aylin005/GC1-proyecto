#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include "openglclass.h"

class LightClass
{
private:
	OpenGLClass* OpenGL;
	float m_diffuseColor[4];
	float m_direction[3];
public:
	//LightClass(OpenGLClass* OpenGL);
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	void GetDiffuseColor(float*);
	void GetDirection(float*);
};

#endif

#pragma once
#ifndef _WATERSHADERCLASS_H_
#define _WATERSHADERCLASS_H_
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <GLFW/glfw3.h>
#include "Shader.h"

class WaterShaderClass : virtual public Shader
{
private:
    OpenGLClass* OpenGL;
    HWND hwnd;
    GLuint m_shaderProgram;
    GLint m_waterTextureLocation;
    GLint m_timeLocation;
protected:
    void BindAttributeLocation()
    {

    }


public:
    // Al mismo tiempo que obtenemos los parametros, ejecutamos el constructor de la clase padre
    WaterShaderClass(OpenGLClass* OpenGL, HWND hwnd) : Shader(OpenGL, hwnd)
    {
        this->OpenGL = OpenGL;
        this->hwnd = hwnd;
    }
    // Al mismo tiempo que obtenemos los parametros, ejecutamos el constructor de la clase padre
    // y luego brindamos los archivos para inicializar el shader
    WaterShaderClass(OpenGLClass* OpenGL, HWND hwnd, const char* vsFilename, const char* fsFilename) : Shader(OpenGL, hwnd)
    {
        this->OpenGL = OpenGL;
        this->hwnd = hwnd;
        Initialize(vsFilename, fsFilename);
    }
    GLuint getShaderID()
    {
        return m_shaderProgram;
    }

    ~WaterShaderClass()
    {}
};

#endif