#include "BGCollisionSetter.h"
#include <iostream>
#include "rapidjson/rapidjson.h"
#include "RapidJsonHelper.h"
#include "BoxCollider.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="collisionBoxJsonFileName">BGXollisoinのファイルパス</param>
BGCollisionSetter::BGCollisionSetter(const char* collisionBoxJsonFileName)
    :Actor(Tag::BackGround)
{
	if (!JsonFileLoad(collisionBoxJsonFileName))
	{
		std::cout << "当たり判定ファイル" << collisionBoxJsonFileName << " 読み込み失敗" << std::endl;
		exit(0);
	}
}

/// <summary>
//  デストラクタ
/// </summary>
BGCollisionSetter::~BGCollisionSetter()
{
}

/// <summary>
/// Jsonファイルのロード
/// </summary>
/// <param name="collisionBoxJsonFileName">BGXollisoinのファイルパス</param>
/// <returns>
/// true  : 読み込み成功
/// false : 読み込み失敗
/// </returns>
bool BGCollisionSetter::JsonFileLoad(const char* collisionBoxJsonFileName)
{
	rapidjson::Document doc;
	if (!openJsonFile(doc, collisionBoxJsonFileName))
	{
		return false;
	}

	if (!IsExistArrayName(doc, "data"))
	{
		printf("JSONファイルパースエラー : %s\n", collisionBoxJsonFileName);
		return false;
	}

	auto collisions = doc["data"].GetArray();
	int collisionSize = collisions.Size();

	// JSONから読み取った内容から CollisionAABBの位置・スケールをすべて取得
	// BoxColliderとして登録
	for (int i = 0; i < collisionSize; i++)
	{
		BoxCollider* pEntryBox;
		AABB col;
		std::string itemName;
		Vector3 position, scale;

		// UE4の基本ボックスの最大、最小値のデフォルト値
		Vector3 max(50, 50, 50);
		Vector3 min(-50, -50, -50);

		// JSONから一とスケール読み込み
		position.x = ForceGetFloat(collisions[i]["position_x"]);
		position.y = ForceGetFloat(collisions[i]["position_y"]);
		position.z = ForceGetFloat(collisions[i]["position_z"]);

		scale.x = ForceGetFloat(collisions[i]["scale_x"]);
		scale.y = ForceGetFloat(collisions[i]["scale_y"]);
		scale.z = ForceGetFloat(collisions[i]["scale_z"]);

		// 読み込んだ位置&スケールから当たり判定ボックスの最大最小求める
		col.mMax = scale * max + position;
		col.mMin = scale * min + position;

		// 当たり判定をPhysicsに登録
		pEntryBox = new BoxCollider(this);
		pEntryBox->SetObjectBox(col);
	}

	return true;
}
