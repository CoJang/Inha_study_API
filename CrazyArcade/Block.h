#pragma once
#include "Objects.h"

class Block : public AnimObject
{
private:
	bool IsDestructible;

public:
	Block();
	~Block();

	void Render(HDC front, HDC back, bool ColliderDraw);
	void Update();
	void UpdateFrame();

	void CreateItem();
	void SetColliderState(bool isactive);

	inline void SetDestructible(bool destructible) { IsDestructible = destructible; };
	inline bool GetDestructible() { return IsDestructible; };
};