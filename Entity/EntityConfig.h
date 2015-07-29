#pragma once

// 对象类型
enum EEntityType
{
	EEntityType_Player = 0,	// 玩家
	EEntityType_Hero,     //英雄
	EEntityType_Item,      //item
	EEntityType_Equip,
	EEntityType_Entity,			// 逻辑对象
	EEntityType_Max = 0xFF,	// uint8
};
