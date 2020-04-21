#include <Windows.h>
#include <tchar.h>

#define MAX_COUNT 100

// WndProc이라는 이름은 정해진것이 아니다 맘대로 써도된다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"ApiBase";


struct DrawData {
	int x, y;
	char type; // 0이면 사각형, 0이 아니면 원
};

DrawData g_pos_list[MAX_COUNT];
int g_pos_count = 0;


// 윈도우 프로그램의 시작점
// hInstance는 지금 실행되고 있는 프로그램의 인스턴스 핸들, 두번째는 바로 앞에 실행된 핸들 통상 NULL, 명령행인자, 윈도우를 보여주는 형태의 플래그
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	/*
	1. 윈도우 클래스를 만든다.
	2. 윈도우 객체를 생성한다.
	3. 윈도우를 화면에 보여준다.
	4. 입력장치로부터 입력신호를 받는다(메시지 루프를 돈다)
	WndProc에서 작업을 처리한다
	*/

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

// 간단하게 클릭하면 박스생기게 하는거 예제
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC h_dc;
	HPEN h_pen;
	HGDIOBJ h_old_pen;


	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	// 없어졌던게 다시 생겨야할때 WM_PAINT 메시지가 온다
	case WM_PAINT:
		PAINTSTRUCT ps;
		 h_dc = BeginPaint(hWnd, &ps);

		 h_pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		 h_old_pen = SelectObject(h_dc, h_pen);


		 // 배열 인덱싱 이렇게 a[i] 이렇게 하면 매 연산마다 a + i 형식으로 값을 불러와서 되게 비효율적이다
		 // 그래서 배열의 포인터를 얻은다음
		 // 다음과 같이 하자 ++ 형식으로 하는게 더 좋다
		 DrawData* dp = g_pos_list;

		for (int i = 0; i < g_pos_count; i++) {
			if(g_pos_list[i].type) 
				Ellipse(h_dc, dp->x - 15, dp->y - 15, dp->x + 15, dp->y + 15);
			else
				Rectangle(h_dc, dp->x - 15, dp->y - 15, dp->x + 15, dp->y + 15);
			dp++;
		}

		SelectObject(h_dc, h_old_pen);
		DeleteObject(h_pen);
		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, h_dc);
		break;
	case WM_LBUTTONDOWN: 
		// getDC로 만든거는 없어질수있다.
		 h_dc = GetDC(hWnd);


		// 마우스 클릭 메시지가 왔을때
		// wParam에는 조합키가 오고
		// lParam에는 좌표값이 온다. word의 하이에는 y좌표 low에는 x좌표
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (g_pos_count < MAX_COUNT) {
			g_pos_list[g_pos_count].x = x;
			g_pos_list[g_pos_count].y = y;
			g_pos_list[g_pos_count].type = wParam & MK_CONTROL; // 0또는 8
			g_pos_count++;
		}
		

		// 색깔 DeleteObject해야한다.
		 h_pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

		// 밑에서 h_pen을 해제하는데 그러면 h_dc가 없어진 펜을 가지게 되므로 문제가됨.. 
		// SelectObject의 반환값은 그전의 그 펜임 그래서 h_old_pen에 넣고 밑에 SelectObject에 한번더 넣어주는 코드를 넣어줘야함
		 h_old_pen = SelectObject(h_dc, h_pen);

		// wParam에는 여러가지 값이 들어가있어서 
		// 비트연산자로 특정한 키가 클릭되었는지 확인한다.
		if (wParam & MK_CONTROL) {
			Ellipse(h_dc, x - 15, y - 15, x + 15, y + 15);
		}
		else {
			Rectangle(h_dc, x - 15, y - 15, x + 15, y + 15);
		}
		
		SelectObject(h_dc, h_old_pen);
		DeleteObject(h_pen);
		ReleaseDC(hWnd, h_dc);
		break;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}