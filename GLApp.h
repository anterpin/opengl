#pragma once

#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <GL\glew.h>
#include <fstream>
#include "Input.h"



class GLApp {
	HWND hWnd;
	MSG msg;
	HDC hDeviceContext;
	HGLRC hGLRenderContext;
	DWORD mWindowStyle;
public:
	static GLApp* getMainDisplay(GLApp* disp = nullptr, bool test = false) {
		static GLApp* m_ptrDisplay = nullptr;
		if (test) {
			m_ptrDisplay = disp;
		}
		return m_ptrDisplay;
	}
	GLApp(LPSTR title, INT nCmdShow) : GLApp(title, nCmdShow, 800, 600) {}
	GLApp(LPSTR title, INT nCmdShow, INT width, INT height, HINSTANCE hInstance = GetModuleHandle(NULL)) {
		hWnd = NULL;
		msg = {};
		hGLRenderContext = NULL;
		mWindowStyle = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_R;

		createConsole();
		createWin(title, hInstance, width, height);
		showWin(nCmdShow);
		createContext();
		getMainDisplay(this, true);
	}
	~GLApp() {
		Close();
		DestroyWindow(hWnd);
	}
	void getSize(float& w, float& h) {
		int wi, he;
		getSize(wi, he);
		w = (float)wi;
		h = (float)he;
	}
	void getSize(int& w, int& h) {
		RECT r;
		GetWindowRect(hWnd, &r);

		h = r.bottom - r.top;
		w = r.right - r.left;
	}
private:
	void createConsole() {
		AllocConsole();

		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);

	}
	static void OutErrorMsg(const char* error) {
		MessageBox(NULL, error, NULL, NULL);
	}
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_DESTROY: {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		}

		case WM_CLOSE:
			if (MessageBox(hwnd, "Really quit?", "Message Box", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hwnd);
				PostQuitMessage(0);
			}
			// Else: User canceled. Do nothing.
			break;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW - 4));

			EndPaint(hwnd, &ps);
			break;
		}
		case WM_KEYDOWN : 
			Input::keyDown(wParam);

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);;

		}
		return 0;
	}
	void createWin(LPSTR str, HINSTANCE hInstance, INT width, INT height) {


		LPCSTR CLASS_NAME = "Sample Window Class";

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = WindowProc;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszClassName = CLASS_NAME;
		wc.lpszMenuName = NULL;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


		if (!RegisterClassEx(&wc)) {
			OutErrorMsg("Could not register win class \n");
			exit(-1);
		}




		hWnd = CreateWindowEx(
			0,					//optional Window Style
			CLASS_NAME,			//class name
			str, // window name
			mWindowStyle,// window style/mode
			(GetSystemMetrics(SM_CXFULLSCREEN) - width) / 2,		// x
			(GetSystemMetrics(SM_CYFULLSCREEN) - height) / 2,		// y
			width,		// w
			height,		// h
			NULL,				//parent win
			NULL,				//menu
			hInstance,			// hInstance
			NULL			//additional application data
		);

		if (hWnd == NULL) {
			OutErrorMsg("Could not create window \n");
			exit(-1);
		}
	}
	void showWin(INT nCmdShow) {
		ShowWindow(hWnd, nCmdShow);
	}
	void createContext() {
		hDeviceContext = GetDC(hWnd);

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
		/*pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.iLayerType = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.cAuxBuffers = 0;*/

		pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
			32,                        //Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                        //Number of bits for the depthbuffer
			8,                        //Number of bits for the stencilbuffer
			0,                        //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};


		int  pixelFormat;
		pixelFormat = ChoosePixelFormat(hDeviceContext, &pfd);
		if (!SetPixelFormat(hDeviceContext, pixelFormat, &pfd)) {
			OutErrorMsg("Did not set Pixel Format \n");
			exit(-1);
		}

		hGLRenderContext = wglCreateContext(hDeviceContext);

		if (!wglMakeCurrent(hDeviceContext, hGLRenderContext)) {
			OutErrorMsg("Could not create Context \n");
			exit(-1);
		}


		glewExperimental = GL_FALSE;
		if (glewInit()) {
			OutErrorMsg("Could not Init Glew\n");
			exit(-1);
		}

		//HGLRC hContext = wglCreateContext(hDeviceContext);
		//wglMakeCurrent(hDeviceContext, hContext);

		//printf("%d\n", glGetString(GL_VERSION));
		//wglDeleteContext(hContext);
	}
public:
	void Close() {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hGLRenderContext);
		ReleaseDC(hWnd, hDeviceContext);
	}
	inline bool isClosed() {
		SwapBuffers(hDeviceContext);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return GetMessage(&msg, NULL, 0, 0);
	}
};
