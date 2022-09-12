#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "DebugText.h"
#include "Audio.h"
#include "DebugCamera.h"
#include "LightGroup.h"
#include "FBXGeneration.h"
#include "Object3D.h"
#include "PlayerBullet.h"

#include <vector>
#include <array>

class PlayerBullet;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	int intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH);


	//弾発射用関数
	void Attack();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText;	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	DebugCamera* camera = nullptr;
	Sprite* spriteBG = nullptr;
	ParticleManager* particleMan = nullptr;

	LightGroup* lightGroup = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;

	Object3d* objTurret = nullptr;
	Object3d* objLife = nullptr;

	PlayerBullet* Bullet = nullptr;
	PlayerBullet* bullet_ = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	
	Model* modelTurret = nullptr;
	Model* modelLife = nullptr;
	Model* Bullet_Model = nullptr;

	bool direction = false;
	bool speedBoost = false;

	Model* enemyModel = nullptr;

	//座標(Position)
	XMFLOAT3 TurretPos = { 0.0f, 0.0f, 0.0f };

	std::array<Object3d*, 4> enemyArray{ {} };
	std::array<bool, 4> enemyBool{ {false, false, false, false} };

	std::array<float, 4> x{ {0.0f, 0.0f, 0.0f, 0.0f} };
	std::array<float, 4> z{ {0.0f, 0.0f, 0.0f, 0.0f} };
	std::array<int, 4> timer{ {0,0,0,0} };
	std::array<float, 4> timeToTarget{ {0.0f, 0.0f, 0.0f} };
};

