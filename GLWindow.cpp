/*

Outracks GLWrapper
Tiny OpenGL wrapping framework for demo use.

You are free to redistribute, change and use this code without charge.
The code is provided "as is", with no warranties or support.

*/

#include "includes/GLWrapper.h"
#include "windows.h"

namespace GLWrapper
{

	bool OPENGL_IS_INITED = false;

	void Window::BASS_replaySong(void)
	{
		basswrap_play_seconds(0.0);
	}

	void Window::BASS_changeSong(char *filename, float startTime)
	{
		basswrap_unload();
		basswrap_load(filename);
		basswrap_play(startTime);
	}
	void Window::BASS_playSong(char* fileName, float startTime)
	{
		basswrap_init(hWnd);
		basswrap_load(fileName);
		basswrap_play(startTime);
	}

	void Window::BASS_getSyncData(float* buffer)
	{
		basswrap_getsync(buffer);
	}

	bool Window::update()
	{
		MSG msg;
		PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
		if(msg.message == WM_QUIT)  {
			return false;
		}	

		glFlush();
		SwapBuffers(hDc);		// Putt nytt framebuffer på skjermen
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return true;
	}


	Window::~Window()
	{
		wglMakeCurrent(hDc, NULL);
		wglDeleteContext(hRC);
		DestroyWindow(hWnd);
		UnregisterClass(CLASSNAME, GetModuleHandle(NULL));
	}

	Window::Window(int width, int height, int colorDepth, bool fullScreen, char* title)
	{
		CLASSNAME = title;
		HINSTANCE hInstance = GetModuleHandle(0);
		DWORD dwStyle;
		windowClass.cbSize = sizeof(WNDCLASSEX);        // size of the WNDCLASSEX structure.
		windowClass.style = CS_HREDRAW | CS_VREDRAW;    // style of the window.
		windowClass.lpfnWndProc = WndProc;              // Address to the windows procedure.
		windowClass.cbClsExtra = 0;                     // Extra class information.
		windowClass.cbWndExtra = 0;                     // Extra window information.
		windowClass.hInstance = GetModuleHandle(NULL);  // Handle of application Instance.
		windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);// Handle of application Icon.
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);// mouse cursor
		windowClass.hbrBackground = NULL;               // background color.
		windowClass.lpszMenuName = NULL;                // name of the main menu.
		windowClass.lpszClassName = title;			// window class name.
		windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);// icon when minimized.
		if (!RegisterClassEx(&windowClass)) 
		{
			MessageBox(HWND_DESKTOP, "Unable to register window class", "GLWindow::GLWindow", MB_OK);
			return;
		}

		if (fullScreen)
		{
			dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			DEVMODE dmSettings;	
			memset(&dmSettings,0,sizeof(dmSettings));
			if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
			{
				MessageBox(HWND_DESKTOP, "Unable to get displaysettings", "Error", MB_OK);
				return;
			}
			dmSettings.dmPelsWidth = width;
			dmSettings.dmPelsHeight = height;
			dmSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			dmSettings.dmBitsPerPel = colorDepth;
			// Bytter til fullscreen med settings fra over
			int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);
			if (result !=DISP_CHANGE_SUCCESSFUL)	
			{ 
				MessageBox(HWND_DESKTOP, "Unable to switch to fullscreen", "GLWindow::GLWindow", MB_OK);
				return;
			}
			ShowCursor(FALSE);
		}
		else
		{
			dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | \
				WS_SYSMENU | WS_CLIPCHILDREN | \
				WS_CLIPSIBLINGS;
		}

		RECT rWindow;
		rWindow.left	= 0;
		rWindow.right	= width;
		rWindow.top	    = 0;
		rWindow.bottom	= height;
		AdjustWindowRect( &rWindow, dwStyle, false);	


		hWnd = CreateWindowEx(NULL,// The extended window style.
			title,// window Class name.
			title,// window name.
			dwStyle,// window style.
			0, 0,// window x, y coordinate.
			rWindow.right  - rWindow.left, // window width
			rWindow.bottom - rWindow.top, //window height.
			NULL,// handle to parent window.
			NULL,// menu.
			hInstance,// handle to app instance.
			NULL);   // pointer to window creation data.

		if (hWnd == NULL) return;

		hDc=GetDC(hWnd);

		// Setter opp pixelformatdescriptor (infor om colordepth, buffere osv)
		static PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),   // size of structure.
				1,                               // always 1.
				PFD_DRAW_TO_WINDOW |             // support window
				PFD_SUPPORT_OPENGL |             // support OpenGl
				PFD_DOUBLEBUFFER,                // support double buffering
				PFD_TYPE_RGBA,                   // support RGBA
				colorDepth,                      // 32 bit color mode
				0, 0, 0, 0, 0, 0,                // ignore color bits
				0,                               // no alpha buffer
				0,                               // ignore shift bit
				0,                               // no accumulation buffer
				0, 0, 0, 0,                      // ignore accumulation bits.
				16,                              // number of depth buffer bits.
				8,                               // number of stencil buffer bits.
				0,                               // 0 means no auxiliary buffer
				PFD_MAIN_PLANE,                  // The main drawing plane
				0,                               // this is reserved
				0, 0, 0 };                       // layer masks ignored.

			int nPixelFormat = NULL;
			nPixelFormat = ChoosePixelFormat(hDc, &pfd);  // Velger pixelformat
			if (nPixelFormat == NULL)
			{
				MessageBox(hWnd, "Unable to find a suitable pixelformat", "GLWindow::GLWindow", MB_OK);
				return;
			}

			if (!SetPixelFormat(hDc, nPixelFormat, &pfd) )	// setter pixelformat
			{		
				MessageBox(hWnd, "Unable to set pixel format", "GLWindow::GLWindow", MB_OK);
				return;
			}

			hRC = wglCreateContext(hDc);		// lager rendering context
			if (hRC == NULL)
			{
				MessageBox(hWnd, "Unable to create HRC", "GLWindow::GLWindow", MB_OK);
				return;
			}

			if (!wglMakeCurrent(hDc, hRC))	// tar rendering context i bruk
			{
				MessageBox(hWnd, "Unable to select HRC", "GLWindow::GLWindow", MB_OK);
				return;
			}

			// Viser vinduet, og setter fokus til det++
			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd); 
			SetForegroundWindow(hWnd);
			SetFocus(hWnd);


			glViewport(0, 0, width, height);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_NORMALIZE);
			glEnable(GL_CULL_FACE);

			OPENGL_IS_INITED = true;

			return;
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)  {
			// Gjør at bildet ikke blir flimrete når vi resizer vinduet
			case WM_ERASEBKGND:
				return 0;
			case WM_CLOSE:
			case WM_KEYDOWN:
				switch(wParam)  {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
				}	
				break;
			case WM_DESTROY:
				PostQuitMessage( 0 );
				return 0;
				break;
			case WM_SIZE:              
				// ignorer resizing, fixed window
				break;
		}
		return (DefWindowProc(hwnd, message, wParam, lParam));
	}
}