#include "GameScene.h"
#include "FBXGeneration.h"
#include "FbxLoader/FbxLoader.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(spriteBG);
	safe_delete(lightGroup);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio * audio)
{
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
	FBXGeneration::SetCamera(camera);

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return ;
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
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objTurret = Object3d::Create();

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelTurret = Model::CreateFromOBJ("chr_sword");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objTurret->SetModel(modelTurret);

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objGround->SetPosition({ 0.0f, 0.0f, 0.0f });
	objTurret->SetPosition({ 0.0f, 10.0f, -28.5f });
	//objTurret->SetPosition({ (objTurret->GetPosition().x - (cosf(XMConvertToRadians(objTurret->GetRotation().y)) * 0.5f)), 10.0f, (objTurret->GetPosition().z + (sinf(XMConvertToRadians(objTurret->GetRotation().y)) * 0.5f)) });

	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });
	objGround->SetScale({ 100.0f, 0.0f, 100.0f });
	objTurret->SetScale({ 3.0f, 3.0f, 3.0f });

	//objTurret->SetRotation({ 0.0f, 180.0f, 0.0f });

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/tex1.png");

	// ライト生成
	lightGroup = LightGroup::Create();

	// カメラ注視点をセット
	camera->SetTarget({0.0f, 0.0f, 0.0f});
	camera->SetUp({ 0, 1, 0 });
	//camera->SetDistance(128.0f);
}

void GameScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
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

	//objTurret->SetPosition({ (objTurret->GetPosition().x - (cosf(XMConvertToRadians(objTurret->GetRotation().y)) * 0.5f)), 10.0f, (objTurret->GetPosition().z + (sinf(XMConvertToRadians(objTurret->GetRotation().y)) * 0.5f)) });
	//objTurret->SetRotation({ 0.0f, objTurret->GetRotation().y + 1.0f, 0.0f });
	
	if (direction)
	{
		objTurret->SetPosition({ (/*objTurret->GetPosition().x +*/ (cosf(XMConvertToRadians(objTurret->GetRotation().y)) * 50.0f)), 10.0f, (/*objTurret->GetPosition().z +*/ (sinf(XMConvertToRadians(objTurret->GetRotation().y)) * 50.0f)) });
		objTurret->SetRotation({ 0.0f, objTurret->GetRotation().y - 1.0f, 0.0f });
	}
	else
	{
		objTurret->SetPosition({ (/*objTurret->GetPosition().x +*/ (cosf(XMConvertToRadians(objTurret->GetRotation().y)) * 50.0f)), 10.0f, (/*objTurret->GetPosition().z +*/ (sinf(XMConvertToRadians(objTurret->GetRotation().y)) * 50.0f)) });
		objTurret->SetRotation({ 0.0f, objTurret->GetRotation().y + 1.0f, 0.0f });
	}

	objTurret->Update();

	camera->Update();

	lightGroup->Update();
	particleMan->Update();

	objSkydome->Update();
	objGround->Update();

	//Debug Start
	char msgbuf[256];
	char msgbuf2[256];
	char msgbuf3[256];

	sprintf_s(msgbuf, 256, "X: %f\n", objTurret->GetPosition().x);
	sprintf_s(msgbuf2, 256, "Y: %f\n", objTurret->GetPosition().y);
	sprintf_s(msgbuf3, 256, "Z: %f\n", objTurret->GetPosition().z);
	OutputDebugStringA(msgbuf);
	OutputDebugStringA(msgbuf2);
	OutputDebugStringA(msgbuf3);
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

	// 3D Object Drawing
	//objSkydome->Draw();
	//objGround->Draw();
	objTurret->Draw();

	// パーティクルの描画
	particleMan->Draw(cmdList);

	Object3d::PostDraw();
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
