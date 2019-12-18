
// OpenGLLab2View.cpp : implementation of the COpenGLLab2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGLLab2.h"
#endif

#include "OpenGLLab2Doc.h"
#include "OpenGLLab2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLLab2View

IMPLEMENT_DYNCREATE(COpenGLLab2View, CView)

BEGIN_MESSAGE_MAP(COpenGLLab2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// COpenGLLab2View construction/destruction
bool drag = false;
float prevX = 0;
float prevY = 0;
COpenGLLab2View::COpenGLLab2View() noexcept
{
	// TODO: add construction code here

}

COpenGLLab2View::~COpenGLLab2View()
{
}

BOOL COpenGLLab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLLab2View drawing

void COpenGLLab2View::OnDraw(CDC* pDC)
{
	COpenGLLab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	m_glRenderer.DrawScene(pDC);
}


// COpenGLLab2View printing

BOOL COpenGLLab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLLab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLLab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COpenGLLab2View diagnostics

#ifdef _DEBUG
void COpenGLLab2View::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLLab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLLab2Doc* COpenGLLab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLLab2Doc)));
	return (COpenGLLab2Doc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLLab2View message handlers


int COpenGLLab2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void COpenGLLab2View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL COpenGLLab2View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void COpenGLLab2View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}

void COpenGLLab2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


BOOL COpenGLLab2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	bool bShift = GetKeyState(VK_SHIFT) & 0x8000;
	bool bCtrl = GetKeyState(VK_CONTROL) & 0x8000;
	bool bEsc = GetKeyState(VK_ESCAPE) & 0x8000;
	bool bSpace = GetKeyState(VK_SPACE) & 0x8000;

	if (zDelta > 0.0)
		if (bShift)
			m_glRenderer.m_Ry += 5.0;
		else if (bCtrl)
			m_glRenderer.m_Rx += 5.0;
		else if (bEsc)
			m_glRenderer.m_x += 0.2;
		else if (bSpace)
			m_glRenderer.m_y += 0.2;
		else
			m_glRenderer.m_z += 0.2;
	if (zDelta < 0.0)
		if (bShift)
			m_glRenderer.m_Ry -= 5.0;
		else if (bCtrl)
			m_glRenderer.m_Rx -= 5.0;
		else if (bEsc)
			m_glRenderer.m_x -= 0.2;
		else if (bSpace)
			m_glRenderer.m_y -= 0.2;
		else
			m_glRenderer.m_z -= 0.2;

	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGLLab2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	/*if (drag)
	{
		m_glRenderer.m_Ry += (point.x - prevX) * 0.4;
		if (m_glRenderer.m_Rx < 60 && (point.y - prevY) * 0.4>0)
		{
			m_glRenderer.m_Rx += (point.y - prevY) * 0.4;
			if (m_glRenderer.m_Rx > 60)
			{
				m_glRenderer.m_Rx = 60;
			}
		}
		if (m_glRenderer.m_Rx > -120 && (point.y - prevY) * 0.4 < 0)
		{
			m_glRenderer.m_Rx += (point.y - prevY) * 0.4;
			if (m_glRenderer.m_Rx < -120)
			{
				m_glRenderer.m_Rx = -120;
			}
		}
		CView::OnMouseMove(nFlags, point);
		Invalidate();
	}
	prevX = point.x;
	prevY = point.y;*/
	if (m_glRenderer.mouseDown) {
		if (m_glRenderer.mouseStart.x < point.x) {
			m_glRenderer.m_Ry += 10.0;
			m_glRenderer.mouseStart = point;
		}
		else if (m_glRenderer.mouseStart.x > point.x) {
			m_glRenderer.m_Ry -= 10.0;
			m_glRenderer.mouseStart = point;
		}
		else if (m_glRenderer.mouseStart.y < point.y) {
			if (m_glRenderer.m_Rx < 70)
				m_glRenderer.m_Rx += 20.0;
			m_glRenderer.mouseStart = point;
		}
		else if (m_glRenderer.mouseStart.y > point.y) {
			if (m_glRenderer.m_Rx > -120)
				m_glRenderer.m_Rx -= 20.0;
			m_glRenderer.mouseStart = point;
		}
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}


void COpenGLLab2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_glRenderer.mouseDown = true;
	m_glRenderer.mouseStart = point;
	CView::OnLButtonDown(nFlags, point);
}


void COpenGLLab2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_glRenderer.mouseDown = false;
	CView::OnLButtonUp(nFlags, point);
}


void COpenGLLab2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_ADD)
		m_glRenderer.m_z += 0.2;
	if (nChar == VK_SUBTRACT)
		m_glRenderer.m_z -= 0.2;

	bool bShift = GetKeyState(VK_SHIFT) & 0x8000;
	if (nChar == VK_RIGHT)
		if (bShift)
			m_glRenderer.m_Ry -= 5.0;
		else
			m_glRenderer.m_x += 0.2;

	if (nChar == VK_LEFT)
		if (bShift)
			m_glRenderer.m_Ry += 5.0;
		else
			m_glRenderer.m_x -= 0.2;

	if (nChar == VK_UP)
		if (bShift)
			m_glRenderer.m_Rx += 5.0;
		else
			m_glRenderer.m_y += 0.2;

	if (nChar == VK_DOWN)
		if (bShift)
			m_glRenderer.m_Rx -= 5.0;
		else
			m_glRenderer.m_y -= 0.2;
	if (nChar == '1')
	{
		m_glRenderer.ang1 += 10;
	}
	else if (nChar == '2')
	{
		m_glRenderer.ang1 -= 10;
	}
	else if (nChar == '3')
	{
		m_glRenderer.ang2 += 10;
	}
	else if (nChar == '4')
	{
		m_glRenderer.ang2 -= 10;
	}
	else if (nChar == 'A')
	{
		m_glRenderer.lx += 0.1;
	}
	else if (nChar == 'S')
	{
		m_glRenderer.ly += 0.1;
	}
	else if (nChar == 'D')
	{
		m_glRenderer.lz += 0.1;
	}
	else if (nChar == 'Y')
	{
		m_glRenderer.lx -= 0.1;
	}
	else if (nChar == 'X')
	{
		m_glRenderer.ly -= 0.1;
	}
	else if (nChar == 'C')
	{
		m_glRenderer.lz -= 0.1;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
