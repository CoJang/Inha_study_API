#include "stdafx.h"
#include "Item.h"
#include "CollisionManager.h"
#include "ImageManager.h"
#include "Block.h"

extern Singleton* singleton;

Block::Block()
{
	Pos = { -1, -1 };
	ImagePivot = { 0, 0 };
	ColPivot = { 52, 72 };
	ColliderSize = { 48, 48 };

	IsColliderActive = true;
	IsDestructible = false;
}

Block::~Block()
{
}

void Block::Render(HDC front, HDC back, bool ColliderDraw)
{
	if (Pos.x == -1 || Pos.y == -1) return;

	AnimObject::Render(front, back, ColliderDraw);
}

void Block::Update()
{
	AnimObject::Update();
}

void Block::UpdateFrame()
{
	AnimObject::UpdateFrame();
}

void Block::CreateItem()
{
	int RandomNum = GenerateRandomNum(ITEM_BOMB, ITEM_FLASK + 2);
	if (RandomNum > ITEM_FLASK) return;

	if (NETWORKMANAGER->GetNetworkType() != HOST) return;
	
	Packet temp; temp.head = ITEM; temp.Pos = Pos; temp.Power = RandomNum;
	NETWORKMANAGER->SendPacket(temp);
	Item* tempItem = new Item(Pos, RandomNum);
	ITEM_VECTOR.push_back(tempItem);
}

void Block::SetColliderState(bool isactive)
{
	IsColliderActive = isactive;

	if (!IsColliderActive)
	{
		for (int i = 0; i < BLOCK_VECTOR.size(); i++)
		{
			if (this == BLOCK_VECTOR[i])
			{
				CreateItem();
				SetPos({ -1, -1 });
				BLOCK_VECTOR.erase(BLOCK_VECTOR.begin() + i);
			}
		}
	}
}
