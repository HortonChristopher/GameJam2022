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

#include <vector>
#include <array>

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

	int intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH);

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

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	
	Model* modelTurret = nullptr;
	Model* modelLife = nullptr;

	bool direction = false;
	bool speedBoost = false;

	//Model* enemyModel = nullptr;

	float playerSpeed = 0.0f;
	float playerSpeedLevel1 = 1.0;
	float playerSpeedLevel2 = 2.0;
	float playerSpeedLevel3 = 3.0;
	float playerLevel = 1;

	std::array<Enemy*, 4> enemyArray{ {} };
	std::array<bool, 4> enemyBool{ {false, false, false, false} };

	std::array<float, 4> x{ {0.0f, 0.0f, 0.0f, 0.0f} };
	std::array<float, 4> z{ {0.0f, 0.0f, 0.0f, 0.0f} };
	std::array<int, 4> timer{ {0,0,0,0} };
	std::array<float, 4> timeToTarget{ {0.0f, 0.0f, 0.0f} };
	std::array<int, 4> RNG{ {0,0,0,0} };
};

