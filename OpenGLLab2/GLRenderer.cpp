#include "pch.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
//#include "GL/GLAUX.H"
//#include "GL\glut.h"

CGLRenderer::CGLRenderer(void)
{
	m_z = 0;
	m_y = 0;
	m_x = 0;
	m_Rx = 0;
	m_Ry = 0;
	m_Rz = 0;
	ang1 = 0;
	ang2 = 0;
	mouseDown = false;
	mouseStart = { 0, 0 };


	lx = 0;
	ly = 0;
	lz = 0;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClearColor(0.15, 0.17, 0.17, 0.0);
	glClearColor(45. / 255, 53. / 255, 61. / 255, 0.0);
	glEnable(GL_DEPTH_TEST);
	PrepareLighting();
	PrepareMaterials();
	glEnable(GL_NORMALIZE);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(6.0, 3.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//Postavljanje svetla
	//float light1_position[] = { -20.0, 20.0, 10.0, 1.0 };
	//float spot_direction[] = { 1.0, -1.0, -1.0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

	//DrawAxes(10.0);
	////glTranslatef(0, 0, -2);
	//glRotatef(m_Rx, 1.0, 0.0, 0.0);
	//glRotatef(m_Ry, 0.0, 1.0, 0.0);
	//matObject.Select();
	////DrawCube(2);
	//DrawLopta(1);
	//DrawPyramid(2, 2, 6);
	DrawScene();
	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene()
{
	glTranslatef(8 * m_x, 8 * m_y - 6 * 0.2 * 8, -5.0 * 8 + 8 * m_z);
	glTranslatef(0, 2 * 8, 0);
	glRotatef(m_Rx + 5 * 6, 1.0, 0.0, 0.0);
	glRotatef(m_Ry, 0.0, 1.0, 0.0);
	glRotatef(m_Rz, 0.0, 0.0, 1.0);
	glTranslatef(0, -2 * 8, 0);
	//glTranslatef(0, 0, -3.0);
	//DrawAxes(10.0);
	float width = 4.0;
	float height = width * 1.5;

	float r_sphere = width / 4;

	float r_cone = width / 3;
	float cone_height = width;

	float r_cylinder = width / 3;
	float cylinder_height = width;

	float r_base = width * 4;
	float base_height = width / 2;

	float cube_width = width * 0.8;
	float cube_height = width;

	float r_hSphere = 1.5 * height;

	glPushMatrix();
	//-------------------------------------
	glFrontFace(GL_CCW);
	//odbacujemo poligone koji se vide sa zadnje strane
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glRotatef(90, 0.0, 1.0, 0.0);
	//glColor3f(0, 0.6, 0);
	//DrawValjak(r_cylinder, cylinder_height);
	//matBase.Select();
	matObject.Select();


	PrepareLighting();

	glLineWidth(1.0);
	DrawAxes(100);

	//crtamo kutiju, sobu
	matObject.Select();
	float edgeOfBox = 10 * width;

	glTranslatef(0, edgeOfBox, 0);
	DrawBox(edgeOfBox);
	glTranslatef(0, -edgeOfBox, 0);

	glTranslatef(0, -r_hSphere, 0);
	DrawHSphere(r_hSphere);
	glTranslatef(0, 2 * r_hSphere, 0);

	//iskljucujemo odbacivanje da ne bismo imali transparentne objekte, sve osim sphere
	glDisable(GL_CULL_FACE);

	glColor3f(0.5, 0.5, 0.5);

	matPyramid.Select();
	glTranslatef(0, width / 4, 0);
	DrawCylinder(r_cylinder, cylinder_height, 8);
	glTranslatef(0, cylinder_height, 0);

	matObject.Select();
	DrawSphere(r_sphere);
	glTranslatef(0, r_sphere, 0);

	//DrawCone(r_cone, cone_height);
	/*DrawPyramid(r_cone, cone_height, 6);
	glTranslatef(0, cone_height, 0);

	DrawSphere(r_sphere);
	glTranslatef(0, r_sphere, 0);*/

	glPushMatrix();

	glRotatef(45, 0.0, 0.0, 1);
	glRotatef(ang1, 0.0, 0.0, 1.0);
	glTranslatef(0, r_sphere, 0);
	glRotatef(ang2, 0.0, 1.0, 0.0);
	//glColor3f(1, 1, 0);

	DrawCube(width / 1.2, height);
	matObject.Select();
	glTranslatef(0, height, 0);

	//glColor3f(0.5, 0.5, 0.5);
	DrawSphere(r_sphere);
	glTranslatef(0, r_sphere, 0);

	glPushMatrix();
	glRotatef(45, 0.0, 0.0, 1);
	glTranslatef(0, r_sphere, 0);

	DrawPyramid(r_cone, cone_height, 6);
	glTranslatef(0, cone_height, 0);

	DrawSphere(r_sphere);
	glTranslatef(0, 2 * r_sphere, 0);

	DrawCylinder(r_cylinder, cylinder_height, 8);
	glTranslatef(0, cylinder_height, 0);

	matSvetlo.Select();
	DrawSphere(r_sphere);
	matObject.Select();

	//SVETLO
	//Aktiviramo zeleno svetlo iz sfere
	ActivateLight();

	glPopMatrix();

	glRotatef(-45, 0.0, 0.0, 1);
	glTranslatef(0, r_sphere, 0);

	DrawPyramid(r_cone, cone_height, 6);
	glTranslatef(0, cone_height, 0);

	DrawSphere(r_sphere);
	glTranslatef(0, 2 * r_sphere, 0);

	DrawCylinder(r_cylinder, cylinder_height, 8);
	glTranslatef(0, cylinder_height, 0);

	DrawSphere(r_sphere);
	//glTranslatef(0, 2 * r_sphere, 0);

	glPopMatrix();

	glRotatef(-45, 0.0, 0.0, 1);
	glTranslatef(0, r_sphere, 0);

	DrawCube(width / 1.2, height);
	glTranslatef(0, height, 0);

	DrawSphere(r_sphere);
	//glTranslatef(0, 2 * r_sphere, 0);

	glPopMatrix();
}

void CGLRenderer::DrawHSphere(float r)
{
	glTranslatef(0, r, 0);
	//glutSolidSphere(r, 100, 100);
	int step = 3;
	glBegin(GL_QUAD_STRIP);
	for (int alpha = 0; alpha < 90; alpha += step) {
		for (int beta = 0; beta <= 360; beta++) {
			float normX = cos(beta * TO_RAD) * cos(alpha * TO_RAD);
			float normY = sin(alpha * TO_RAD);
			float normZ = sin(beta * TO_RAD) * cos(alpha * TO_RAD);

			glNormal3f(normX, normY, normZ);

			glVertex3f(r * normX, r * normY, r * normZ);

			float normX1 = cos(beta * TO_RAD) * cos((alpha + step) * TO_RAD);
			float normY1 = sin((alpha + step) * TO_RAD);
			float normZ1 = sin(beta * TO_RAD) * cos((alpha + step) * TO_RAD);
			glNormal3f(normX1, normY1, normZ1);

			glVertex3f(r * normX1, r * normY1, r * normZ1);
		}
	}
	glEnd();
	matGreen.Select();
	glBegin(GL_LINES);
	for (int alpha = 0; alpha <= 90; alpha += 3)
	{
		for (int beta = 0; beta <= 360; beta += 11.25)
		{
			float normX = cos(beta * TO_RAD) * cos(alpha * TO_RAD);
			float normY = sin(alpha * TO_RAD);
			float normZ = sin(beta * TO_RAD) * cos(alpha * TO_RAD);

			glVertex3f(r * cos(beta * TO_RAD) * cos(alpha * TO_RAD), r * sin(alpha * TO_RAD), r * sin(beta * TO_RAD) * cos(alpha * TO_RAD));
			glVertex3f(r * cos(beta * TO_RAD) * cos(alpha * TO_RAD) + normX, r * sin(alpha * TO_RAD) + normY, r * sin(beta * TO_RAD) * cos(alpha * TO_RAD) + normZ);
		}
	}
	glEnd();
	matObject.Select();
	glTranslatef(0, -r, 0);
}

void CGLRenderer::ActivateLight()
{
	//SVETLO
	//glTranslatef(0,  rl, 0);
	float light1_ambient[] = { 0.25, 0.5, 0.25, 1.0 };
	float light1_diffuse[] = { 0.5, 0.8, 0.5, 1.0 };
	float light1_specular[] = { 0.5, 1.0, 0.5, 1.0 };
	GLfloat light1_position[] = { 0, 0, 0 ,1 };
	GLfloat light1_direction[] = { 0, 1, 0 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT7, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT7, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light1_direction);

	glLightfv(GL_LIGHT7, GL_POSITION, light1_position);

	//Slabljenje
	glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 20);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 7.0);

	//Aktiviranje
	glEnable(GL_LIGHT7);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawBox(float w)
{
	//radimo segmentaciju zbog osvetljenosti, veliki kvadrat iz vise manjih sastavljen
	glBegin(GL_QUADS);
	float step = 1;
	glNormal3f(0, -1, 0);
	for (float i = -w; i < w; i += step)
	{
		for (float j = -w; j < w; j += step)
		{
			glVertex3f(i + step, -w, j + step);
			glVertex3f(i + step, -w, j);
			glVertex3f(i, -w, j);
			glVertex3f(i, -w, j + step);
		}
	}
	glNormal3f(0, 1, 0);
	for (float i = -w; i < w; i += step)
	{
		for (float j = -w; j < w; j += step)
		{
			glVertex3f(i + step, w, j + step);
			glVertex3f(i + step, w, j);
			glVertex3f(i, w, j);
			glVertex3f(i, w, j + step);
		}
	}
	glNormal3f(1, 0, 0);
	for (float i = -w; i < w; i += step)
	{
		for (float j = -w; j < w; j += step)
		{
			glVertex3f(-w, i, j + step);
			glVertex3f(-w, i, j);
			glVertex3f(-w, i + step, j);
			glVertex3f(-w, i + step, j + step);
		}
	}
	glNormal3f(-1, 0, 0);
	for (float i = -w; i < w; i += step)
	{
		for (float j = -w; j < w; j += step)
		{
			glVertex3f(w, i + step, j + step);
			glVertex3f(w, i + step, j);
			glVertex3f(w, i, j);
			glVertex3f(w, i, j + step);
		}
	}
	glNormal3f(0, 0, -1);
	for (float i = -w; i < w; i += step)
	{
		for (float j = -w; j < w; j += step)
		{
			glVertex3f(i, j + step, -w);
			glVertex3f(i, j, -w);
			glVertex3f(i + step, j, -w);
			glVertex3f(i + step, j + step, -w);
		}
	}
	glNormal3f(0, 0, 1);
	for (float i = -w; i < w; i += step)
	{
		for (float j = -w; j < w; j += step)
		{
			glVertex3f(i + step, j + step, w);
			glVertex3f(i + step, j, w);
			glVertex3f(i, j, w);
			glVertex3f(i, j + step, w);
		}
	}
	glEnd();
}

void CGLRenderer::DrawCube(float w, float h)
{
	glBegin(GL_QUADS);
	//zadnja
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0, -w / 2);

	//prednja
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, h, w / 2);
	glVertex3f(-w / 2, h, w / 2);

	//leva
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2, h, -w / 2);

	//desna
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, 0, -w / 2);

	//donja
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0, -w / 2);

	//gornja
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, h, w / 2);
	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2, h, -w / 2);
	glEnd();

	matGreen.Select();
	glBegin(GL_LINES);

	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(-w / 2, h, -w / 2 - 1);

	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, h, -w / 2 - 1);

	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2, 0, -w / 2 - 1);

	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0, -w / 2 - 1);


	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0, w / 2 + 1);

	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, 0, w / 2 + 1);

	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2, h, w / 2 + 1);

	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2, h, w / 2 + 1);


	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2 - 1, 0, -w / 2);

	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2 - 1, 0, w / 2);

	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2 - 1, h, w / 2);

	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(-w / 2 - 1, h, -w / 2);


	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2 + 1, h, -w / 2);

	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2 + 1, h, w / 2);

	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2 + 1, 0, w / 2);

	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2 + 1, 0, -w / 2);


	glVertex3f(w / 2, 0, -w / 2);
	glVertex3f(w / 2, 0 - 1, -w / 2);

	glVertex3f(w / 2, 0, w / 2);
	glVertex3f(w / 2, 0 - 1, w / 2);

	glVertex3f(-w / 2, 0, w / 2);
	glVertex3f(-w / 2, 0 - 1, w / 2);

	glVertex3f(-w / 2, 0, -w / 2);
	glVertex3f(-w / 2, 0 - 1, -w / 2);


	glVertex3f(-w / 2, h, -w / 2);
	glVertex3f(-w / 2, h + 1, -w / 2);

	glVertex3f(-w / 2, h, w / 2);
	glVertex3f(-w / 2, h + 1, w / 2);

	glVertex3f(w / 2, h, w / 2);
	glVertex3f(w / 2, h + 1, w / 2);

	glVertex3f(w / 2, h, -w / 2);
	glVertex3f(w / 2, h + 1, -w / 2);

	glEnd();
	matObject.Select();
}
void CGLRenderer::DrawCone(float r, float h)
{
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, -1, 0);
	glVertex3f(0.0f, 0, 0.0f);
	for (int i = 0; i <= 360; i += 60)
	{
		glVertex3f(r * cos(i * TO_RAD), 0, r * sin(i * TO_RAD));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	float sinF = sin(atan((float)r / h));
	float cosF = cos(atan((float)r / h));
	for (int i = 360; i >= 0; i -= 60) {
		glNormal3f(cos(i * TO_RAD) * cosF, sinF, sin(i * TO_RAD) * cosF);
		glVertex3f(0, h, 0);
		glVertex3f(r * cos(i * TO_RAD), 0, r * sin(i * TO_RAD));
	}
	glEnd();
	matGreen.Select();
	glBegin(GL_LINES);
	for (int i = 360; i >= 0; i -= 60) {
		glVertex3f(0, h, 0);
		glVertex3f(0 + cos(i * TO_RAD) * cosF, h + sinF, 0 + sin(i * TO_RAD) * cosF);
		glVertex3f(r * cos(i * TO_RAD), 0, r * sin(i * TO_RAD));
		glVertex3f(r * cos(i * TO_RAD) + cos(i * TO_RAD) * cosF, 0 + sinF, r * sin(i * TO_RAD) + sin(i * TO_RAD) * cosF);
	}
	glEnd();
	matObject.Select();
}
void CGLRenderer::DrawSphere(float r)
{
	glTranslatef(0, r, 0);
	//glutSolidSphere(r, 100, 100);
	//ugao fi na slici je od -90 do 90, a teta pun krug prolazi
	glBegin(GL_QUAD_STRIP);
	for (int j = -90; j < 90; j++) {
		for (int i = 0; i <= 360; i++) {
			glNormal3f(cos(i * TO_RAD) * cos(j * TO_RAD), sin(j * TO_RAD), sin(i * TO_RAD) * cos(j * TO_RAD));
			glVertex3f(r * cos(i * TO_RAD) * cos(j * TO_RAD), r * sin(j * TO_RAD), r * sin(i * TO_RAD) * cos(j * TO_RAD));
			glNormal3f(cos(i * TO_RAD) * cos((j + 1) * TO_RAD), sin((j + 1) * TO_RAD), sin(i * TO_RAD) * cos((j + 1) * TO_RAD));
			glVertex3f(r * cos(i * TO_RAD) * cos((j + 1) * TO_RAD), r * sin((j + 1) * TO_RAD), r * sin(i * TO_RAD) * cos((j + 1) * TO_RAD));
		}
	}
	glEnd();
	glTranslatef(0, -r, 0);
}

void CGLRenderer::DrawCylinder(float r, float h, float n)
{
	float seg = 2 * PI / n;

	glBegin(GL_TRIANGLE_FAN);

	glNormal3f(0, +1, 0);
	glVertex3f(0.0f, h, 0.0f);
	for (int i = 0; i <= n; i++)
	{
		glVertex3f(r * cos(i * seg), h, r * sin(i * seg));
	}
	glEnd();


	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, -1, 0);
	glVertex3f(0.0f, 0, 0.0f);
	for (int i = 0; i <= n; i++)
	{
		glVertex3f(r * cos(i * seg), 0, r * sin(i * seg));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; i++) {
		glNormal3f(cos(i * seg), 0, sin(i * seg));
		glVertex3f(r * cos(i * seg), h, r * sin(i * seg));
		glVertex3f(r * cos(i * seg), 0, r * sin(i * seg));
	}
	glEnd();

	matGreen.Select();
	glBegin(GL_LINES);
	for (int i = 0; i <= n; i++) {
		glVertex3f(r * cos(i * seg), h, r * sin(i * seg));
		glVertex3f(r * cos(i * seg) + cos(i * seg), h, r * sin(i * seg) + sin(i * seg));
		glVertex3f(r * cos(i * seg), 0, r * sin(i * seg));
		glVertex3f(r * cos(i * seg) + cos(i * seg), 0, r * sin(i * seg) + sin(i * seg));
	}
	glEnd();
	matObject.Select();
}

void CGLRenderer::DrawBase(float r, float h, int n)
{
	glTranslatef(0, -h, 0);
	float seg = 2 * PI / n;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, 1, 0);
	glVertex3f(0.0f, h, 0.0f);
	for (int i = 0; i <= n; i++)
	{
		glVertex3f(r * cos(-i * seg), h, r * sin(-i * seg));
	}
	glEnd();


	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, -1, 0);
	glVertex3f(0.0f, 0, 0.0f);
	for (int i = 0; i <= n; i++)
	{
		glVertex3f(r * cos(-i * seg), 0, r * sin(-i * seg));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; i++)
	{
		glNormal3f(r * cos(i * seg), h, r * sin(i * seg));
		glVertex3f(r * cos(i * seg), h, r * sin(i * seg));
		glNormal3f(r * cos(i * seg), 0, r * sin(i * seg));
		glVertex3f(r * cos(i * seg), 0, r * sin(i * seg));
	}
	glEnd();

	glTranslatef(0, h, 0);
}

void CGLRenderer::DrawPyramid(float r, float h, int n)
{
	glTranslatef(0, 0, 0);
	float seg = 2 * PI / n;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, -1, 0);
	glVertex3f(0.0f, 0, 0.0f);
	for (int i = 0; i <= n; i++)
	{
		glVertex3f(r * cos(i * seg), 0, r * sin(i * seg));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	float sinAlpha = sin(atan((float)r / h));
	float cosAlpha = cos(atan((float)r / h));
	glVertex3f(0.0f, h, 0.0f);
	for (int i = 0; i <= n + 1; i++)
	{
		glNormal3f(cos(i * seg) * cosAlpha, sinAlpha, r * sin(i * seg) * cosAlpha);
		glVertex3f(0, h, 0);
		glVertex3f(r * cos(i * seg), 0, r * sin(i * seg));
	}
	glEnd();

	matGreen.Select();
	glBegin(GL_LINES);
	for (int i = 0; i <= n; i++) {
		glVertex3f(0, h, 0);
		glVertex3f(0 + cos(i * seg) * cosAlpha, h + sinAlpha, 0 + sin(i * seg) * cosAlpha);
		glVertex3f(r * cos(i * seg), 0, r * sin(i * seg));
		glVertex3f(r * cos(i * seg) + cos(i * seg) * cosAlpha, 0 + sinAlpha, r * sin(i * seg) + sin(i * seg) * cosAlpha);
	}
	glEnd();
	matObject.Select();
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspect = (double)w / (double)h;
	gluPerspective(40, aspect, 0.1, 200);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::PrepareLighting()
{
	float light1_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	float light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { lx, ly - 1, lz, 0 };
	GLfloat light1_direction[] = { 0,+1,0 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);

	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	//Slabljenje
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 7.0);

	//Aktiviranje
	glEnable(GL_LIGHT1);

	float light2_ambient[] = { 0.25, 0.25, 0.25, 1.0 };
	float light2_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	float light2_specular[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat light2_position[] = { lx + 1, ly, lz, 0 };
	GLfloat light2_direction[] = { -1, 0,0 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);

	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

	//Slabljenje
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 7.0);

	glEnable(GL_LIGHT2);

	float light3_ambient[] = { 0.125, 0.125, 0.125, 1.0 };
	float light3_diffuse[] = { 0.250, 0.250, 0.250, 0.250 };
	float light3_specular[] = { 0.250, 0.250, 0.250, 0.250 };
	GLfloat light3_position[] = { lx, ly, lz - 1, 0 };
	GLfloat light3_direction[] = { 0, 0, 1 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_direction);

	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);

	//Slabljenje
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 7.0);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::PrepareMaterials()
{
	/*matCone.SetAmbient(0.2, 0.2, 0.2, 1.0);
	matCone.SetDiffuse(0.8, 0.8, 0.8, 1.0);
	matCone.SetSpecular(1.0, 1.0, 1.0, 0.0);
	matCone.SetEmission(0.0, 0.0, 0.0, 0.0);
	matCone.SetShininess(16.0);*/

	matBase.SetAmbient(0.1, 0.8, 0.1, 0.0);
	matBase.SetDiffuse(0, 1, 0, 0.0);
	matBase.SetSpecular(0.5, 0.9, 0.5, 0.0);
	matBase.SetEmission(0.0, 0.0, 0.0, 0.0);
	matBase.SetShininess(16.0);

	matPyramid.SetAmbient(0.8, 0.8, 0, 0.0);
	matPyramid.SetDiffuse(1, 1, 0, 0.0);
	matPyramid.SetSpecular(1.0, 1.0, 1.0, 0.0);
	matPyramid.SetEmission(0.0, 0.0, 0.0, 0.0);
	matPyramid.SetShininess(16.0);

	matObject.SetAmbient(119 / 255., 119 / 255., 119 / 255., 1.0);
	matObject.SetDiffuse(0.33, 0.33, 0.33, 1.0);
	matObject.SetSpecular(188 / 255., 188 / 255., 188 / 255., 1.0);
	matObject.SetEmission(0.0, 0.0, 0.0, 1.0);
	matObject.SetShininess(255.0);


	matSvetlo.SetAmbient(0, 0, 0, 0.0);
	matSvetlo.SetDiffuse(0, 0, 0, 0.0);
	matSvetlo.SetSpecular(0, 0, 0, 0.0);
	matSvetlo.SetEmission(0.5, 1.0, 0.5, 0.0);
	matSvetlo.SetShininess(16.0);

	matRed.SetAmbient(1.0, 0, 0, 0.0);
	matRed.SetDiffuse(1.0, 0, 0, 0.0);
	matRed.SetSpecular(1.0, 0, 0, 0.0);
	matRed.SetEmission(0.0, 0.0, 0.0, 0.0);
	matRed.SetShininess(16.0);

	matGreen.SetAmbient(0.0, 1, 0, 0.0);
	matGreen.SetDiffuse(0.0, 1, 0, 0.0);
	matGreen.SetSpecular(0.0, 1, 0, 0.0);
	matGreen.SetEmission(0.0, 0, 0.0, 0.0);
	matGreen.SetShininess(16.0);

	matBlue.SetAmbient(0.0, 0, 1, 0.0);
	matBlue.SetDiffuse(0.0, 0, 1, 0.0);
	matBlue.SetSpecular(0.0, 0, 1, 0.0);
	matBlue.SetEmission(0.0, 0, 0.0, 0.0);
	matBlue.SetShininess(16.0);


	/*matCube.SetAmbient(1.0, 0.0, 0.2, 0.0);
	matCube.SetDiffuse(0.1, 0.3, 0.7, 0.0);
	matCube.SetSpecular(0.8,0.8,0.8, 0.0);
	matCube.SetEmission(0.0, 0.0, 0.0, 0.0);
	matCube.SetShininess(32.0);

	matCylinder.SetAmbient(0, 1, 0, 0.0);
	matCylinder.SetDiffuse(0, 1, 0, 0.0);
	matCylinder.SetSpecular(1.0, 1.0, 1.0, 0.0);
	matCylinder.SetEmission(0.0, 0.0, 0.0, 0.0);
	matCylinder.SetShininess(16.0);

	matSphere.SetAmbient(0, 1, 0, 0.0);
	matSphere.SetDiffuse(0, 1, 0, 0.0);
	matSphere.SetSpecular(1.0, 1.0, 1.0, 0.0);
	matSphere.SetEmission(0.0, 0.0, 0.0, 0.0);
	matSphere.SetShininess(16.0);*/
}

void CGLRenderer::DrawAxes(double len)
{
	glBegin(GL_LINES);
	//Y osa
	glColor3f(0, 1, 0);
	matGreen.Select();
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);

	//Z osa
	glColor3f(0, 0, 1);
	matBlue.Select();
	glVertex3f(0, 0, 0);
	glVertex3f(-100, 0, 0);

	//X osa
	glColor3f(1, 0, 0);
	matRed.Select();
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);

	glEnd();
}
