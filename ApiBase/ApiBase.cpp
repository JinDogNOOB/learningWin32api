#include <Windows.h>
#include <tchar.h>


// WndProc�̶�� �̸��� ���������� �ƴϴ� ����� �ᵵ�ȴ�.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM iParam);

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"ApiBase";

// ������ ���α׷��� ������
// hInstance�� ���� ����ǰ� �ִ� ���α׷��� �ν��Ͻ� �ڵ�, �ι�°�� �ٷ� �տ� ����� �ڵ� ��� NULL, ���������, �����츦 �����ִ� ������ �÷���
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	/*
	1. ������ Ŭ������ �����.
	2. ������ ��ü�� �����Ѵ�.
	3. �����츦 ȭ�鿡 �����ش�.
	4. �Է���ġ�κ��� �Է½�ȣ�� �޴´�(�޽��� ������ ����)
	WndProc���� �۾��� ó���Ѵ�
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM iParam) {
	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, iParam));
}