#pragma once
#include "Objects.h"

#define WM_ASYNC WM_USER + 2
#define MAX_CHAT 5

class ClientClass
{
private:
	WSADATA wsadata;
	SOCKET s;
	SOCKADDR_IN addr;
	TCHAR msg[512];
	TCHAR wbuff[128];
	int Cnt, size, msgLen;
	char buff[128];
	vector<wstring> ChatLog;
	vector<POINT> WhiteStoneContainer;
	vector<POINT> BlackStoneContainer;
	bool IsMyTurn;
private:
	HDC* BackBuffer;
	HDC* FrontBuffer;
public:
	ClientClass();
	~ClientClass();

	void InitClient(HWND hWnd, HDC* Front, HDC* Back);
	void Render();
	void ReadMessage(WPARAM wParam);
	void CheckKeyDown(WPARAM wParam);
	void MouseDown(POINT MousePos);

	void DrawLine(POINT start, POINT end);
	void DrawGrid(POINT pos, int Num);

	wstring ParseMessage(TCHAR* Msg);

	void DrawCircle(POINT pos, int R);
};

