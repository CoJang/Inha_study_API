#pragma once
#include "stdafx.h"

enum SceneType
{
	TITLE,
	GAME,
	GAMEOVER
};

enum ButtonType
{
	NEXT,
	PREVIOUS,
	QUIT,

	UNKNOWN = 99
};

class Scene
{
protected:
	wstring ID;
	SceneType type;

	HFONT myFont;
	HFONT oldFont;
	HDC* hdc;
public:
	Scene();
	virtual ~Scene();
	void InitScene(HDC* device) { hdc = device; };

	virtual void SetID(wstring input) { ID = input; };
	virtual wstring GetID() { return NULL; };
	virtual void ShowID() {};

	virtual void DrawButtons() {};
	virtual ButtonType CheckClick(POINT mpos, int flag) { return UNKNOWN; };
	virtual void CheckKeyDown(WPARAM wParam) { };

	int GetType() { return type; };
	void SetType(SceneType input) { type = input; };

	virtual void Update() = 0;
	virtual void Render() = 0;
};

