#include <windows.h>


/**********************************************************/
/******************    Used classes    ********************/
/**********************************************************/


class LineDrawer {
private:
	int fromX, fromY, toX, toY;
	boolean shouldDraw;
	HDC hdc;
public:

	LineDrawer(HDC hdc) {
		this->hdc = hdc;
	}

	void startLine(int x, int y) {
		fromX = x;
		fromY = y;

	}

	void finishLine(int x, int y) {
		toX = x;
		toY = y;
		shouldDraw = true;
	}

	void drawIfNecessary() {
		if (shouldDraw)
		{
			MoveToEx(hdc, fromX, fromY, NULL);
			LineTo(hdc, toX, toY);
			shouldDraw = false;
		}
	}
};

class TextDrawer {
private:
	HDC hdc;
	int x, y;
	bool isMarked;
public:
	TextDrawer(HDC hdc) {
		this->hdc = hdc;
	}

	void markText(int x, int y) {
		this->x = x;
		this->y = y;
		this->isMarked = true;
	}

	void showIfMarked() {
		if (isMarked)
		{
			TCHAR buffer[100];
			wsprintf(buffer, L"X: %d, Y: %d", x, y);
			TextOut(hdc, x, y, buffer, wcslen(buffer));
			isMarked = false;
		}
	}

};

/**********************************************************/
/**********************************************************/

void showWindow(HINSTANCE);
void initializeWindowClass(HINSTANCE);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const wchar_t* WINDOW_CLASS_NAME = L"Sample Window Class";
const wchar_t* WINDOW_CAPTION = L"Drawing window";


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR pCmdLine, int nCmdShow)
{

	initializeWindowClass(hInstance);
	showWindow(hInstance);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void initializeWindowClass(HINSTANCE hInstance) {
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClass(&wc);
}

void showWindow(HINSTANCE hInstance) {

	HWND hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, WINDOW_CAPTION, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 480, 360, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
}



/****************************************************************************/
/* ************            Window processing function             ***********/
/****************************************************************************/



HDC  hdcDrawingArea;
LineDrawer *lineDrawer;
TextDrawer *textDrawer;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_LBUTTONDOWN:
		lineDrawer->startLine(LOWORD(lParam), HIWORD(lParam));
	case WM_LBUTTONUP:
		lineDrawer->finishLine(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		textDrawer->markText(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		lineDrawer->drawIfNecessary();
		textDrawer->showIfMarked();
		break;

	case WM_CREATE:
		hdcDrawingArea = GetDC(hwnd);
		lineDrawer = new LineDrawer(hdcDrawingArea);
		textDrawer = new TextDrawer(hdcDrawingArea);
		break;
	case WM_DESTROY:
		ReleaseDC(hwnd, hdcDrawingArea);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}