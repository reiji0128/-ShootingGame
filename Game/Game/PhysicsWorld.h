#pragma once
#include "Game.h"
#include "Tag.h"
#include <unordered_map>
#include <vector>

class PhysicsWorld
{
public:
	struct collidePairs
	{
		Tag pair1;
		Tag pair2;
	};

	PhysicsWorld();
	~PhysicsWorld();

	void AddCollider(class ColliderComponent* collider);               // Collider追加
	void RemoveCollider(class ColliderComponent* collider);            // Collider削除
	void DebugShowBoxLists();                                          // ボックスリスト表示（デバッグ用)
	void Collision();                                                  // コリジョン
	void DebugShowBox();                                               // デバッグ用ボックス表示
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }       // デバッグモード

	void SetOneSideReactionCollisionPair(Tag noReactionType, Tag reactionType); // 当たり判定ペアのセット（片方だけリアクション返すタイプ）
	void SetDualReactionCollisionPair(Tag reaction1, Tag reaction2);            // 当たり判定ペアのセット（両方リアクション）
	void SetSelfReaction(Tag selfreaction);                                     // 同じグループ同士の当たり判定セット

	void ClearOneSidePair();                                           // 当たり判定ペアのクリア
	void ClearDualPair();
	void ClearSelfPair();
	void ClearAllPair();

private:
	void OneReactionMatch(collidePairs cp);
	void DualReactionMatch(collidePairs cp);
	void SelfReactionMatch(Tag type);

	void InitBoxVertices();
	void InitSquareVertices();
	void InitLineVertices();
	void DrawCollisions(std::vector<class ColliderComponent*>& boxs, const Vector3& color);

	bool                      mBoolDebugMode;                          // デバッグモード
	std::vector<collidePairs> mOneSideReactions;                       // 片方だけリアクションを行う当たり判定ペアリスト
	std::vector<collidePairs> mDualReactions;                          // 両方ともリアクションを行う当たり判定ペアリスト
	std::vector<Tag>          mSelfReactions;                          // 同じグループ内での当たり判定を行うリスト
	std::vector<Vector3>      mLineColors;                             // 当たり判定ボックスのカラー
	std::unordered_map<Tag, std::vector<class ColliderComponent*>> mColliders;    // 当たり判定ボックスリスト

	unsigned int mBoxVAO;      // ボックス描画用のVAO 
	unsigned int mSquareVAO;   // 平面描画用のVAO
	unsigned int mLineVAO;     // ライン用VAO
	class Shader* mLineShader; // ライン描画用シェーダー


};