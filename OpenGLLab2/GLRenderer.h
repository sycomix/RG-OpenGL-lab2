#pragma once
#include "CGLMaterial.h"

#define TO_RAD 0.01745329251994329576923690768489
#define TO_DEG 57.295779513082320876798154814105
#define PI 3.141592

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawCube(float w, float h);
	void DrawCone(float r, float h);
	void DrawSphere(float r);
	void DrawCylinder(float r, float h, float n);
	void DrawBase(float r, float h, int n);
	void DrawPyramid(float r, float h, int n);
	void PrepareLighting();
	void PrepareMaterials();
	void DrawAxes(double len);
	void DrawScene();
	void DrawBox(float w);
	void DrawHSphere(float r);
	void ActivateLight();

	float   m_z;
	float   m_x;
	float   m_y;
	float   m_Rx;
	float   m_Ry;
	float   m_Rz;
	bool mouseDown;
	CPoint mouseStart;

	float lx;
	float ly;
	float lz;

	float ang1;
	float ang2;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	CGLMaterial matSvetlo, matRed, matGreen, matBlue;
	CGLMaterial matPyramid, matObject, matBase, matCone; // jer su ostale sve figure istom bojom
};


