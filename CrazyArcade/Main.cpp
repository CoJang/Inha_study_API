#include "stdafx.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Main.h"

#define MAX_LOADSTRING 100

// Global Variables:
extern Singleton* singleton;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CRAZYARCADE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CRAZYARCADE));

    MSG msg;

	// Main message loop:
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// Key Down Sequence
			//singleton->GetSceneManager()->GetInstance()->CheckKeyDown();
		}
	}

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CRAZYARCADE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CRAZYARCADE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static HDC BackBuffer[2];
	static HBITMAP BackBitmap, oldBitmap;
	static POINT MousePos;

    switch (message)
    {
	case WM_CREATE:
		{
			singleton = new Singleton;
			SetTimer(hWnd, 100, ElapseTime, NULL);
			BackBuffer[0] = CreateCompatibleDC(hdc);
			BackBuffer[1] = CreateCompatibleDC(BackBuffer[0]);

			singleton->InitSingleton(hWnd, &BackBuffer[0], &BackBuffer[1]);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
		//double startTime = GetTickCount();
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			BackBitmap = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
			oldBitmap = (HBITMAP)SelectObject(BackBuffer[0], BackBitmap);
			PatBlt(BackBuffer[0], 0, 0, WIN_WIDTH, WIN_HEIGHT, WHITENESS);

			singleton->GetSceneManager()->GetInstance()->Update();
			singleton->GetSceneManager()->GetInstance()->CheckKeyDown();
			singleton->GetSceneManager()->GetInstance()->Render();

			BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, BackBuffer[0], 0, 0, SRCCOPY);
			SelectObject(BackBuffer[0], oldBitmap);

			DeleteObject(oldBitmap);
			DeleteObject(BackBitmap);

			EndPaint(hWnd, &ps);
		//double endTime = GetTickCount();
		//cout << endTime - startTime << endl;
        }
        break;
	case WM_LBUTTONUP:
		MousePos = { LOWORD(lParam), HIWORD(lParam) };
		singleton->GetSceneManager()->GetInstance()->CheckClick(MousePos, 0);
		break;
	case WM_MOUSEMOVE:
		MousePos = { LOWORD(lParam), HIWORD(lParam) };
		singleton->GetSceneManager()->GetInstance()->CheckClick(MousePos, 1);
		break;
	case WM_CHAR:
	{
		if(CURRENT_SCENE->GetType() == TITLE)
		{
			static string input;
			if (wParam != VK_RETURN && wParam != VK_BACK)
			{
				input += wParam;
				CURRENT_SCENE->SetIP(input);
			}
			else if (wParam == VK_BACK && input.size() > 0)
			{
				input.erase(input.begin() + input.size() - 1);
				CURRENT_SCENE->SetIP(input);
			}
			else
			{
				CURRENT_SCENE->SetIP(input);
				input = "";
			}
		} // end of if(CURRENT_SCENE == TITLE)
	} // end of WM_CHAR:
		break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_ACCEPT:
			NETWORKMANAGER->Accept();
			break;
		case FD_READ:
			NETWORKMANAGER->ReadMessage(wParam);
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;
    case WM_DESTROY:
		delete singleton;
		KillTimer(hWnd, 100);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
