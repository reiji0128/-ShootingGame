#pragma once
#include <string>
#include <stdexcept>
#include <iterator>

// ゲーム内のアクター・当たり判定の種類別タグ
// BegienとEndの間に255個まで定義できる

enum class Tag : unsigned char
{
	Begin,				// Tagの開始マーカー(Beginより手前に定義してはならない)

	Player = Begin,
	PlayerBullet,
	Enemy,
	EnemyBullet,
	BackGround,
	Altar,
	Camera,
	Effect,
	Weapon,

	End                 // Tagの終了マーカー　（Endより後に定義してはならない）
};

// 前置インクリメント
Tag& operator ++(Tag& rhs);