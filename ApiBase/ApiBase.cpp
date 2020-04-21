#include <Windows.h>
#include <tchar.h>

#define MAX_COUNT 100

// WndProc�̶�� �̸��� ���������� �ƴϴ� ����� �ᵵ�ȴ�.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
LPCWSTR lpszClass = L"ApiBase";


struct DrawData {
	int x, y;
	char type; // 0�̸� �簢��, 0�� �ƴϸ� ��
};

DrawData g_pos_list[MAX_COUNT];
int g_pos_count = 0;



// �׶����� DC�� �ҷ����� ������ DC �ϴ°� ���� �׷��� �Ǽ��Ҽ��� �ְ� , �� �κ��� Ŭ����ȭ��ų���ִ�.
// �̷��� MFC...?�� ��κ��� ���̷��� �������ֳ�..
// MFC�� ������ ���� WrapperŬ�������� ����,,, ���Ұ� �� �۾��� �س���

// ����� �����ϰ� ����
class DC {
protected:
	HDC mh_dc;
	HWND mh_wnd;

public:

	// ������
	DC(HWND ah_wnd) {
		// ����ڰ� �� ����� ������ DC�� ��´�.
		mh_wnd = ah_wnd;
	}
	// ���� �� ��ݿ� �Ҹ��� ��ĳ�����̳� �������̵��� �����Ͱ����� virtual ������
	virtual ~DC() {
	}

	HGDIOBJ SelectObject(HGDIOBJ ah_gdiobj) {
		// :: �׳� ���ָ� �����Լ��� �ǹ��Ѵ�. ���� �� Ŭ������ ����Լ��� �ƴ� �����Լ�
		return ::SelectObject(mh_dc, ah_gdiobj);
	}

	void Rectangle(int sx, int sy, int ex, int ey) {
		::Rectangle(mh_dc, sx, sy, ex, ey);
	}
	void Ellipse(int sx, int sy, int ex, int ey) {
		::Ellipse(mh_dc, sx, sy, ex, ey);
	}
};


class CClientDC : public DC {
public:
	// ������
	CClientDC(HWND ah_wnd) : DC(ah_wnd){
		// ����ڰ� �� ����� ������ DC�� ��´�.
		mh_dc = GetDC(ah_wnd);
	}
	// �Ҹ���
	~CClientDC() {
		ReleaseDC(mh_wnd, mh_dc);
	}
};

class CPaintDC : public DC {
protected:
	PAINTSTRUCT m_ps;
public:
	CPaintDC(HWND ah_wnd) : DC(ah_wnd) {
		mh_dc = BeginPaint(mh_wnd, &m_ps);
	}

	~CPaintDC() {
		EndPaint(mh_wnd, &m_ps);
	}
};



/*
CClientDC dc(hWnd);
dc.SelectObject(pen);
dc.Ellipse();


*/





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

// �����ϰ� Ŭ���ϸ� �ڽ������ �ϴ°� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC h_dc;
	HPEN h_pen;
	HGDIOBJ h_old_pen;


	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	// ���������� �ٽ� ���ܾ��Ҷ� WM_PAINT �޽����� �´�
	case WM_PAINT:
		PAINTSTRUCT ps;
		 h_dc = BeginPaint(hWnd, &ps);

		 h_pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		 h_old_pen = SelectObject(h_dc, h_pen);


		 // �迭 �ε��� �̷��� a[i] �̷��� �ϸ� �� ���긶�� a + i �������� ���� �ҷ��ͼ� �ǰ� ��ȿ�����̴�
		 // �׷��� �迭�� �����͸� ��������
		 // ������ ���� ���� ++ �������� �ϴ°� �� ����
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
		// getDC�� ����Ŵ� ���������ִ�.
		 h_dc = GetDC(hWnd);


		// ���콺 Ŭ�� �޽����� ������
		// wParam���� ����Ű�� ����
		// lParam���� ��ǥ���� �´�. word�� ���̿��� y��ǥ low���� x��ǥ
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (g_pos_count < MAX_COUNT) {
			g_pos_list[g_pos_count].x = x;
			g_pos_list[g_pos_count].y = y;
			g_pos_list[g_pos_count].type = wParam & MK_CONTROL; // 0�Ǵ� 8
			g_pos_count++;
		}
		

		// ���� DeleteObject�ؾ��Ѵ�.
		 h_pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

		// �ؿ��� h_pen�� �����ϴµ� �׷��� h_dc�� ������ ���� ������ �ǹǷ� ��������.. 
		// SelectObject�� ��ȯ���� ������ �� ���� �׷��� h_old_pen�� �ְ� �ؿ� SelectObject�� �ѹ��� �־��ִ� �ڵ带 �־������
		 h_old_pen = SelectObject(h_dc, h_pen);

		// wParam���� �������� ���� ���־ 
		// ��Ʈ�����ڷ� Ư���� Ű�� Ŭ���Ǿ����� Ȯ���Ѵ�.
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






