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
#include "Enemy.h"
#include "RareEnemy.h"
#include "PlayerBullet.h"
#include "PlayerBullet_R.h"
#include "PlayerBullet_L.h"

#include <vector>
#include <array>
#include <memory>
#include <list>

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

	// パーティクル生成
	void CreateParticles(float x, float z);
	void CreateBombParticles(float x, float z);

	int intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH);
	int circlecircleIntersect(XMFLOAT3 c1, XMFLOAT3 c2, float r1, float r2);

	//弾発射用関数
	void Attack();

	//拡散弾
	void DiffusionAttack_R();

	void DiffusionAttack_L();

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

	RareEnemy* objRareEnemy = nullptr;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	std::list<std::unique_ptr<PlayerBullet_R>> bullets_R;
	std::list<std::unique_ptr<PlayerBullet_L>> bullets_L;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	
	Model* modelTurret = nullptr;
	Model* modelLife = nullptr;
	Model* Bullet_Model = nullptr;

	bool direction = false;
	bool speedBoost = false;

	//Model* enemyModel = nullptr;

	// プレイヤーレベル
	float playerLevel = 1;
	float LevelMax = 3;
	float LevelMin = 1;

	// プレイヤースピード管理
	float playerSpeed = 0.0f;
	float playerSpeedLevel1 = 1.0;
	float playerSpeedLevel2 = 2.0;
	float playerSpeedLevel3 = 3.0;

	// プレイヤーバレットタイプ管理
	bool playerBulletType;
	bool normalBullet;
	bool spreadBullet;

	// プレイヤーボム
	float playerBombGage = 0;
	float BombGageMax = 20;
	bool bombFlag = 0;

	int playerLife = 8;

	std::array<Enemy*, 4> enemyArray{ {} };
	std::array<Sprite*, 9> lifeGauge{ {} };
	Model* enemyModel = nullptr;

	//座標(Position)
	XMFLOAT3 TurretPos = { 0.0f, 0.0f, 0.0f };
	std::array<bool, 4> enemyBool{ {false, false, false, false} };

	std::array<float, 4> x{ {0.0f, 0.0f, 0.0f, 0.0f} };
	std::array<float, 4> z{ {0.0f, 0.0f, 0.0f, 0.0f} };
	std::array<int, 4> timer{ {0,0,0,0} };
	std::array<float, 4> timeToTarget{ {0.0f, 0.0f, 0.0f} };
	std::array<int, 4> RNG{ {0,0,0,0} };

	int testNum = 0;
};

