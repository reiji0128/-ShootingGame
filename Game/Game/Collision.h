#pragma once
#include "Game.h"

// 衝突情報
struct CollisionInfo
{
	//衝突点
	Vector3 mCollisionPoint;

	//押し戻しベクトル
	Vector3 mFixVec;

	//衝突点での法線
	Vector3 mHitNormal;

	//スピード
	float mSpeed;
};

// 球体
struct Sphere
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="radius">半径</param>
	Sphere(const Vector3& center, float radius);

	/// <summary>
	/// 球体内に点が含まれるか？
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <returns>
	/// true  : 含まれている
	/// false : 含まれていない
	/// </returns>
	bool Contains(const Vector3& point) const;

	// 中心位置
	Vector3 mCenter;

	// 半径
	float mRadius;
};

// 線分
struct Line
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Line();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="start">始点座標</param>
	/// <param name="end">終点座標</param>
	Line(const Vector3& start,const Vector3& end);

	// 始点座標
	Vector3 mLineStart;

	// 終点座標
	Vector3 mLineEnd;

	// 正規化した向き
	Vector3 mNormalizeDirection;

	// 線分の長さ
	float mLineLength;

	// 回転可能か？
	bool mRotatable;
};

// 軸並行ボックス(Axis-Aligned Bounding Box)
struct AABB
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AABB();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="min">ボックスの最小座標</param>
	/// <param name="max">ボックスの最大座標</param>
	AABB(const Vector3& min, const Vector3& max);

	/// <summary>
	/// モデル読み込み時の最小・最大の点を求めるのに使用
	/// </summary>
	/// <param name="point">点の座標</param>
	void InitMinMax(const Vector3& point) { mMin = mMax = point; }

	/// <summary>
	/// ボックスの頂点（8個）を再計算する関数
	/// </summary>
	void CalcVertex();

	/// <summary>
	/// 最小・最大座標の更新処理
	/// </summary>
	/// <param name="point">点の座標</param>
	void UpdateMinMax(const Vector3& point);

	/// <summary>
	/// 点がボックス内に含まれるか?
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <returns>
	/// true  : 含まれている
	/// false : 含まれていない
	/// </returns>
	bool Contains(const Vector3& point) const;

	/// <summary>
	/// 点との最小距離を求める関数
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <returns>点との最小距離</returns>
	float MinDistSq(const Vector3& point) const;

	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name="q">回転角度</param>
	void Rotate(const Quaternion& q);

	/// <summary>
	/// スケーリング
	/// </summary>
	/// <param name="x">Xの拡大率</param>
	/// <param name="y">Yの拡大率</param>
	/// <param name="z">Zの拡大率</param>
	void Scaling(float x, float y, float z);

	/// <summary>
	/// スケーリング
	/// </summary>
	/// <param name="scale">拡大率</param>
	void Scaling(float scale);

// セッター //
	/// <summary>
	/// 回転を許可するか？
	/// </summary>
	/// <param name="value"></param>
	void SetArrowRotate(bool value) { mIsRotatable = value; }

	// ボックス最小座標
	Vector3 mMin;

	// ボックス最大座標
	Vector3 mMax;

	// ボックスの頂点座標を格納する配列
	Vector3 mVertex[8];

	// 回転するかしないか？
	bool mIsRotatable = false;
};

// 壁面
struct Wall
{
	// 角度
	enum AngleEnum
	{
		Angle0 = 0,
		Angle90 = 90,
		Angle180 = 180,
		Angle270 = 270
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Wall();

	/// <summary>
	/// 点が壁に含まれているか？
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <returns>
	/// true  : 含まれている
	/// false : 含まれている
	/// </returns>
	bool Contains(const Vector3& point);

	/// <summary>
	/// z軸に水平壁を作成な壁を作成
	/// </summary>
	/// <param name="start">始点座標</param>
	/// <param name="end">終点座標</param>
	/// <param name="wallHeight">壁の高さ</param>
	/// <returns>Wallのインスタンス</returns>
	static Wall CreateWall(const Vector3& start, const Vector3 end, float wallHeight);

	/// <summary>
	/// 坂道の作成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="zRotateDegree">斜面の角度</param>
	/// <param name="horizonLen">長さ</param>
	/// <param name="height">高さ</param>
	/// <param name="slopeWidth">幅</param>
	/// <returns>Wallのインスタンス</returns>
	static Wall CreateSlope(const Vector3& pos, Wall::AngleEnum zRotateDegree, float horizonLen, float height, float slopeWidth);	//坂道作成

	void CalcAABB();

	// 壁全体を含むAABB
	AABB mWallAABB;

	// 壁の法線
	Vector3 mNormal;

	// 壁の開始点
	Vector3 mStart;

	// 壁の終了点
	Vector3 mEnd;

	// 壁の頂点
	Vector3 mWallVertex[4];

	// スケーリング係数
	Vector3 mScale;

	//壁面の平面方程式のd値
	float mPlaneD;

	//z軸回転
	float mZRotate;

	//傾斜角
	float mSlopeAngle;

	//スロープか？
	bool mIsSlope;
};

/// <summary>
/// AABB 同士の衝突
/// </summary>
/// <param name="a">自分のAABB</param>
/// <param name="b">相手のAABB</param>
/// <returns>
/// true  : 衝突している
/// false : 衝突していない
/// </returns>
bool Intersect(const AABB& a, const AABB& b);

/// <summary>
/// 球体とAABBとの衝突
/// </summary>
/// <param name="s">球体のインスタンス</param>
/// <param name="box">AABBのインスタンス</param>
/// <returns>点との最小距離</returns>
bool Intersect(const Sphere& s, const AABB& box);

/// <summary>
/// AABBと壁面の衝突を調べ、めり込み戻りベクトルを返す
/// </summary>
/// <param name="a">AABBのインスタンス</param>
/// <param name="wall">Wallのインスタンス</param>
/// <param name="info">衝突情報</param>
/// <returns>
/// true  : 押し戻し量を返した
/// false : 衝突なし
/// </returns>
bool Intersect(const AABB& a, const Wall& wall, CollisionInfo& info);

/// <summary>
/// AABBと点の衝突
/// </summary>
/// <param name="a">AABBのインスタンス</param>
/// <param name="movablepoint">点の座標</param>
/// <param name="info">衝突情報</param>
/// <returns>
/// true  : 衝突した
/// false : 衝突なし
/// </returns>
bool Intersect(const AABB& a, const Vector3& movablepoint, CollisionInfo& info);

/// <summary>
/// 線分とAABBの衝突
/// </summary>
/// <param name="line">Lineのインスタンス</param>
/// <param name="box">AABBのインスタンス</param>
/// <param name="info">衝突情報</param>
/// <returns>
/// true  : 衝突した
/// false : 衝突なし
/// </returns>
bool Intersect(const Line& line, const AABB& box, CollisionInfo& info);

/// <summary>
/// 壁と線分の衝突情報
/// </summary>
/// <param name="wall">壁のインスタンス</param>
/// <param name="line">線分のインスタンス</param>
/// <param name="info">衝突情報</param>
/// <returns>
/// true  : 衝突した
/// false : 衝突なし
/// </returns>
bool Intersect(const Wall& wall, const Line& line, CollisionInfo& info);

/// <summary>
/// 壁のフチの線とボックスが衝突したかを判定する関数
/// 2D的にZ値を無視して、xy平面上での判定とどれだけめり込んだかを判定する
/// Intersect (AABB, Wall)内で使用することを想定
/// </summary>
/// <param name="a">AABBのインスタンス</param>
/// <param name="edgeLow">下の縁の座標</param>
/// <param name="edgeHi">上之縁の座標</param>
/// <param name="fixVec">めり込み量</param>
/// <returns>
/// true  : 衝突した
/// false : 衝突なし
/// </returns>
bool IntersectWallEdgeFix(const AABB& a, const Vector3& edgeLow, const Vector3& edgeHi, Vector3& fixVec);

/// <summary>
/// AABBのめり込みをもどす量を計算する関数
/// </summary>
/// <param name="movableBox">移動物体 (ex プレーヤー)</param>
/// <param name="fixedBox">移動しない物体（ex ブロック</param>
/// <param name="calcFixVec">移動物体の補正差分ベクトル</param>
void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec);

