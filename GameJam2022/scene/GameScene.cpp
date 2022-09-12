﻿#include "GameScene.h"
#include "FBXGeneration.h"
#include "FbxLoader/FbxLoader.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include "camera.h"

//あ

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(spriteBG);
	safe_delete(lightGroup);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	srand(time(NULL));

	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);

	//collisionManager = CollisionManager::GetInstance();

	// Device set
	FBXGeneration::SetDevice(dxCommon->GetDevice());
	// Camera set
	Object3d::SetCamera(camera);
	Enemy::SetCamera(camera);
	RareEnemy::SetCamera(camera);
	FBXGeneration::SetCamera(camera);

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
		assert(0);
		return;
	}

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// パーティクルマネージャ生成
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objTurret = Object3d::Create();
	objLife = Object3d::Create();
	objRareEnemy = RareEnemy::Create(nullptr, dxCommon);

	for (int i = 0; i < 4; i++)
	{
		enemyArray[i] = Enemy::Create(nullptr, dxCommon);
	}

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelTurret = Model::CreateFromOBJ("chr_sword");
	modelLife = Model::CreateFromOBJ("sphere");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objTurret->SetModel(modelTurret);
	objLife->SetModel(modelLife);
	objRareEnemy->SetModel(modelLife);

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objGround->SetPosition({ 0.0f, 0.0f, 0.0f });
	objTurret->SetPosition({ 0.0f, 10.0f, 0.0f });
	objLife->SetPosition({ 0.0f, 10.0f, 0.0f });

	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });
	objGround->SetScale({ 100.0f, 0.0f, 100.0f });
	objTurret->SetScale({ 3.0f, 3.0f, 3.0f });
	objLife->SetScale({ 3.0f, 3.0f, 3.0f });

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/tex1.png");

	// ライト生成
	lightGroup = LightGroup::Create();

	// カメラ注視点をセット
	camera->SetTarget({ 0.0f, 0.0f, 0.0f });
	camera->SetUp({ 0, 1, 0 });
	//camera->SetDistance(128.0f);
}

void GameScene::Update()
{
	if (input->TriggerKey(DIK_SPACE)) // 撃つとプレーヤーの動く逆になる
	{
		if (direction == true)
		{
			direction = false;
		}
		else
		{
			direction = true;
		}
	}

	if (input->PushKey(DIK_LSHIFT)) // スピードブースト
	{
		speedBoost = true;
	}
	else
	{
		speedBoost = false;
	}

	if (direction) // プレイヤーが回転する方向を制御するブール値
	{
		objTurret->SetPosition({ (/*objTurret->GetPosition().x +*/ (sinf(XMConvertToRadians(objTurret->GetRotation().y - 180.0f)) * 20.0f)), 10.0f, (/*objTurret->GetPosition().z +*/ (cosf(XMConvertToRadians(objTurret->GetRotation().y - 180.0f)) * 20.0f)) });
		if (!speedBoost)
		{
			objTurret->SetRotation({ 0.0f, objTurret->GetRotation().y + playerSpeed, 0.0f }); // playerSpeedLevel = スピード
		}
		else
		{
			objTurret->SetRotation({ 0.0f, objTurret->GetRotation().y + 2.0f, 0.0f });
		}
	}
	else // 逆の方向
	{
		objTurret->SetPosition({ (/*objTurret->GetPosition().x +*/ (sinf(XMConvertToRadians(objTurret->GetRotation().y - 180.0f)) * 20.0f)), 10.0f, (/*objTurret->GetPosition().z +*/ (cosf(XMConvertToRadians(objTurret->GetRotation().y - 180.0f)) * 20.0f)) });
		if (!speedBoost)
		{
			objTurret->SetRotation({ 0.0f, objTurret->GetRotation().y - playerSpeed, 0.0f }); // playerSpeedLevel = スピード
		}
		else
		{
			objTurret->SetRotation({ 0.0f, objTurret->GetRotation().y - 2.0f, 0.0f });
		}
	}

#pragma region プレイヤーレベル管理
	// 自機レベルアップ
	if (enemyArray[0]->enemyDefeated > 4) // デバッグのみ
	{
		playerLevel++;
		enemyArray[0]->enemyDefeated = 0;
	}

	// レベル1
	if (playerLevel == 1)
	{
		playerSpeed = playerSpeedLevel1;
	}
	// レベル2
	else if (playerLevel == 2)
	{
		playerSpeed = playerSpeedLevel2;
	}
	// レベル3
	else if (playerLevel == 3)
	{
		playerSpeed = playerSpeedLevel3;
	}
#pragma endregion

	// 自機強化(拡散弾)

	// レベルダウン
	/*if ()
	{

	}*/

	// パーティクル生成
	if (input->TriggerKey(DIK_RETURN)) // デバッグ全部の敵を殺す
	{
		for (int i = 0; i < 4; i++)
		{
			CreateParticles(enemyArray[i]->GetPosition().x, enemyArray[i]->GetPosition().z);
		}
	}

	for (int i = 0; i < 4; i++) // 敵がライフに命中するとパーティクルを作成します
	{
		if (enemyArray[i]->destruction)
		{
			CreateParticles(enemyArray[i]->particlePosition.x, enemyArray[i]->particlePosition.z);
		}
	}

	objTurret->Update();
	objLife->Update();

	particleMan->Update();

	for (int i = 0; i < 4; i++)
	{
		enemyArray[i]->Update();
	}

	camera->Update();

	lightGroup->Update();

	objSkydome->Update();
	objGround->Update();

#pragma region デバックテキスト
	// プレイヤーのレベルを表示
	std::ostringstream PlayerLevel;
	PlayerLevel << "PlayerLevel:("
		<< std::fixed << std::setprecision(0)
		<< playerLevel << ")";
	debugText->Print(PlayerLevel.str(), 50, 90, 1.0f);

	// プレイヤーの速度を表示
	std::ostringstream PlayerSpeed;
	PlayerSpeed << "PlayerSpeed:("
		<< std::fixed << std::setprecision(1)
		<< playerSpeed << ")";
	debugText->Print(PlayerSpeed.str(), 50, 110, 1.0f);
#pragma endregion

	objRareEnemy->Update();

	//Debug Start
	/*char msgbuf[256];
	char msgbuf2[256];
	char msgbuf3[256];

	sprintf_s(msgbuf, 256, "X: %f\n", objTurret->GetPosition().x);
	sprintf_s(msgbuf2, 256, "Y: %f\n", objTurret->GetPosition().y);
	sprintf_s(msgbuf3, 256, "Z: %f\n", objTurret->GetPosition().z);
	OutputDebugStringA(msgbuf);
	OutputDebugStringA(msgbuf2);
	OutputDebugStringA(msgbuf3);*/
	//Debug End
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	//Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	//Sprite::PostDraw();
	// 深度バッファクリア
	//dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	Object3d::PreDraw(cmdList);
	Enemy::PreDraw(cmdList);
	RareEnemy::PreDraw(cmdList);

	// 3D Object Drawing
	//objSkydome->Draw();
	//objGround->Draw();
	objTurret->Draw();
	objLife->Draw();

	objRareEnemy->Draw();

	for (int i = 0; i < 4; i++)
	{
		enemyArray[i]->Draw();
	}

	// パーティクルの描画
	particleMan->Draw(cmdList);

	Object3d::PostDraw();
	Enemy::PostDraw();
	RareEnemy::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::CreateParticles(float x, float z)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 5.0f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + x;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + 10.0f;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + z;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		particleMan->Add(120, pos, vel, acc, 1.0f, 0.0f);
	}
}

int GameScene::intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH)
{
	XMFLOAT2 circleDistance;

	circleDistance.x = abs(player.x - wall.x);
	circleDistance.y = abs(player.z - wall.z);

	if (circleDistance.x > (rectW / 2.0f + circleR)) { return false; }
	if (circleDistance.y > (rectH / 2.0f + circleR)) { return false; }

	if (circleDistance.x <= (rectW / 2.0f)) { return true; }
	if (circleDistance.y <= (rectH / 2.0f)) { return true; }

	float cornerDistance_sq = ((circleDistance.x - rectW / 2.0f) * (circleDistance.x - rectW / 2.0f)) + ((circleDistance.y - rectH / 2.0f) * (circleDistance.y - rectH / 2.0f));

	return (cornerDistance_sq <= (circleR * circleR));
}
