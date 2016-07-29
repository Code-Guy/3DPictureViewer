#include "mainwidget.h"
#include <QMessageBox>
#include <GL/glew.h>
#include <GL/wglext.h>
#include <Windows.h>

HGLRC _dummy_glctx;
HGLRC _real_glctx;

const int MSAA = 4;
const int WIDTH = 1280;
const int HEIGHT = 960;

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initWidgetProp();
	initOpenGLContext();
	initMultiSample();
	initGlew();

	//初始化FPS计时器
	fpsTime = new QTime;
	fpsTime->start();
}

MainWidget::~MainWidget()
{
	delete fpsTime;

	wglDeleteContext(_dummy_glctx);
	wglDeleteContext(_real_glctx);
}

void MainWidget::logic(float deltaTime)
{

}

void MainWidget::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

}

void MainWidget::paintEvent(QPaintEvent* evt)
{
	tickClock();

	logic(deltaTime);
	render();

	HWND hwnd = (HWND)winId();
	HDC hdc = GetDC(hwnd);
	SwapBuffers(hdc);
	ReleaseDC(hwnd, hdc);
}

void MainWidget::initWidgetProp()//初始化widget的一些属性
{
	setWindowOpacity(1);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_InputMethodEnabled);

	setFixedSize(WIDTH, HEIGHT);
}

void MainWidget::initOpenGLContext()//初始化window界面context
{
	QMessageBox *box = new QMessageBox;
	HWND hwnd = (HWND)box->winId();
	HDC hdc = GetDC(hwnd);
	//[METHOD] SetWindowPixelFormat(HDC)

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                        // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER
		| PFD_SUPPORT_GDI/*|PFD_STEREO*/,    // dwFlags
		PFD_TYPE_RGBA,            // PixelType
		24,                       // ColorBits
		0, 0, 0, 0, 0, 0,         // RGB bits and shifts
		8,                        // cAlphaBits
		0,                        // cAlphaShift
		0, 0, 0, 0, 0,            // Accum
		24,                       // Depth
		8,                        // Stencil
		0,                        // cAuxBuffers
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat;
	pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (pixelFormat == 0) {
		pixelFormat = 1;
		if (DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd) == 0)
			return;
	}

	if (SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE) return;

	//[NOTE] !DO NOT! change check order
	if (!(_dummy_glctx = wglCreateContext(hdc)) || !wglMakeCurrent(hdc, _dummy_glctx))
		return;
}

void MainWidget::initGlew()//初始化Glew
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		exit(-1);
	}
}

bool MainWidget::initMultiSample()//设置MultiSample
{
	HWND hwnd = (HWND)winId();
	HDC hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                        // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER
		| PFD_SUPPORT_GDI/*|PFD_STEREO*/,    // dwFlags
		PFD_TYPE_RGBA,            // PixelType
		24,                       // ColorBits
		0, 0, 0, 0, 0, 0,         // RGB bits and shifts
		8,                        // cAlphaBits
		0,                        // cAlphaShift
		0, 0, 0, 0, 0,            // Accum
		24,                       // Depth
		8,                        // Stencil
		0,                        // cAuxBuffers
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int pixelFormat;
	if (MSAA == -1)
	{
		pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if (pixelFormat == 0) {
			pixelFormat = 1;
			if (DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd) == 0)
				return false;
		}
	}
	else if (!setMultisample(hwnd, MSAA, pixelFormat))
	{
		return false;
	}
	if (SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE)
		return false;

	//[NOTE] !DO NOT! change check order
	if (!(_real_glctx = wglCreateContext(hdc)) || !wglMakeCurrent(hdc, _real_glctx))
		return false;

	if (MSAA != -1)
	{
		glEnable(GL_MULTISAMPLE_ARB);
	}

	return true;
}

bool MainWidget::setMultisample(HWND hWnd, int sampleNum, int &pixelFormat)
{
	// See If The String Exists In WGL!
	if (!wglIsExtensionSupported("WGL_ARB_multisample"))
	{
		return false;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB)
	{
		return false;
	}

	// Get Our Current Device Context
	HDC hDC = GetDC(hWnd);

	int		valid;
	UINT	numFormats;
	float	fAttributes[] = { 0, 0 };

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	int iAttributes[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 16,
		WGL_STENCIL_BITS_ARB, 0,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, sampleNum,
		0, 0
	};

	// First We Check To See If We Can Get A Pixel Format For 4 Samples
	valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);

	// If We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1)
	{
		return true;
	}
	return false;
}

bool MainWidget::wglIsExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported;; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);

		if (p == NULL)
			return false;															// No Match

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"

		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return true;															// Match
	}
}

void MainWidget::tickClock()
{
	deltaTime = fpsTime->elapsed();
	deltaTime /= 1000.0f;
	fpsTime->restart();
}