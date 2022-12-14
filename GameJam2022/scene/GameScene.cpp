#include "GameScene.h"
#include "FBXGeneration.h"
#include "FbxLoader/FbxLoader.h"
#include "PlayerBullet.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include "camera.h"
#include <algorithm>

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

	//オーディオ初期化
	audio->Initialize();

	//音声ロード
	audio->LoadWave("Title.wav");
	audio->LoadWave("Bomb.wav");
	audio->LoadWave("Damage.wav");
	audio->LoadWave("GameScene.wav");
	audio->LoadWave("Heal.wav");
	audio->LoadWave("Kill.wav");
	audio->LoadWave("Result.wav");
	audio->LoadWave("Shot.wav");
	audio->LoadWave("Select.wav");

	audio->PlayWave("Title.wav", Volume_Title, true);

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
	PlayerBullet::SetCamera(camera);

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

	if (!Sprite::LoadTexture(4, L"Resources/PlayerLife/LifeOverlay.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(5, L"Resources/PlayerLife/L1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(6, L"Resources/PlayerLife/L2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(7, L"Resources/PlayerLife/L3.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(8, L"Resources/PlayerLife/L4.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(9, L"Resources/PlayerLife/L5.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(10, L"Resources/PlayerLife/L6.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(11, L"Resources/PlayerLife/L7.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(12, L"Resources/PlayerLife/L8.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(13, L"Resources/BombGage/BombGage0.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(14, L"Resources/BombGage/BombGage2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(15, L"Resources/BombGage/BombGage4.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(16, L"Resources/BombGage/BombGage6.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(17, L"Resources/BombGage/BombGage8.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(18, L"Resources/BombGage/BombGage10.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(19, L"Resources/BombGage/BombGage12.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(20, L"Resources/BombGage/BombGage14.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(21, L"Resources/BombGage/BombGage16.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(22, L"Resources/BombGage/BombGage18.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(23, L"Resources/BombGage/BombGage20.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(24, L"Resources/BombGage/BombGage22.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(25, L"Resources/BombGage/BombGage24.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(26, L"Resources/BombGage/BombGage26.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(27, L"Resources/BombGage/BombGage28.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(28, L"Resources/BombGage/BombGage30.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(29, L"Resources/PlayerLevel/PlayerLevel1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(30, L"Resources/PlayerLevel/PlayerLevel2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(31, L"Resources/PlayerLevel/PlayerLevel3.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(32, L"Resources/Title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(33, L"Resources/result.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(34, L"Resources/Guid1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(35, L"Resources/Guid2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(36, L"Resources/Bomb.png"))
	{
		assert(0);
		return;
	}

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	for (int i = 0; i < 9; i++)
	{
		lifeGauge[i] = Sprite::Create((i + 4), { 1070.0f, 0.0f });
	}
	for (int i = 0; i < 16; i++)
	{
		bombGauge[i] = Sprite::Create((i + 13), { 800.0f, 525.0f });
	}
	for (int i = 0; i < 3; i++)
	{
		playerLevelSprite[i] = Sprite::Create((i + 29), { -20.0f, 525.0f });
	}
	for (int i = 0; i < 4; i++)
	{
		titleEndingSprite[i] = Sprite::Create((i + 32), { 0.0f, 0.0f });
	}
	bombSprite = Sprite::Create(36, { 1095.0f, 545.0f });
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
	modelTurret = Model::CreateFromOBJ("player");
	modelLife = Model::CreateFromOBJ("Core");
	enemyModel = Model::CreateFromOBJ("RareEnemy");
	Bullet_Model = Model::CreateFromOBJ("bullet2");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objTurret->SetModel(modelTurret);
	objLife->SetModel(modelLife);
	objRareEnemy->SetModel(enemyModel);

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objGround->SetPosition({ 0.0f, 0.0f, 0.0f });
	objTurret->SetPosition(TurretPos);
	objLife->SetRotation(lifeRotation);

	for (int i = 0; i < 4; i++)
	{
		int RNG = rand() % 1;
		int RNG2 = rand() % 1;
		if (RNG == 1)
		{
			if (RNG2 == 1)
			{
				enemyArray[i]->SetPosition({ rand() % 101 - 150.0f, 10.0f, rand() % 101 - 150.0f });
			}
			else
			{
				enemyArray[i]->SetPosition({ rand() % 101 + 50.0f, 10.0f, rand() % 101 - 150.0f });
			}
		}
		else
		{
			if (RNG2 == 1)
			{
				enemyArray[i]->SetPosition({ rand() % 101 + 50.0f, 10.0f, rand() % 101 - 150.0f });
			}
			else
			{
				enemyArray[i]->SetPosition({ rand() % 101 + 50.0f, 10.0f, rand() % 101 + 50.0f });
			}
		}
	}

	//objTurret->SetPosition({ (objTurret->GetPosition().x - (cosf(XMConvertToRadians(objTurret->GetRotation().y)) * 0.5f)), 10.0f, (objTurret->GetPosition().z + (sinf(XMConvertToRadians(objTurret->GetRotation().y)) * 0.5f)) });

	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });
	objGround->SetScale({ 100.0f, 0.0f, 100.0f });
	objTurret->SetScale({ 3.0f, 3.0f, 3.0f });
	objLife->SetPosition({ 0.0f, 10.0f ,0.0f });
	objLife->SetScale({ 5.0f, 5.0f, 5.0f }); // 3 3 3


	//for (int i = 0; i < 4; i++)
	//{
		//enemyArray[i]->SetScale({ 5.0f, 5.0f, 5.0f });
	//}

	//objTurret->SetRotation({ 0.0f, 180.0f, 0.0f });

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
	std::ostringstream playerScore;
	std::ostringstream hiScore;

	switch (gamePlayScene)
	{
	case 0:
		if (input->TriggerKey(DIK_SPACE))
		{
			switch (pageNo)
			{
			case 0:
				audio->PlayWave("Select.wav", Volume_Title);
				pageNo = 2;
			case 1:
				break;
			case 2:
				audio->PlayWave("Select.wav", Volume_Title);
				pageNo++;
				break;
			case 3:
				audio->StopWave("Title.wav");
				audio->PlayWave("Select.wav", Volume_Title);
				audio->PlayWave("GameScene.wav", Volume_Title, true);
				gamePlayScene++;
				break;
			}
			
		}
		break;
	case 1:
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
			objTurret->SetPosition({ (/*objTurret->GetPosition().x +*/ (sinf(XMConvertToRadians(objTurret->GetRotation().y)) * 12.5f)), 10.0f, (/*objTurret->GetPosition().z +*/ (cosf(XMConvertToRadians(objTurret->GetRotation().y)) * 12.5f)) });
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
			objTurret->SetPosition({ (/*objTurret->GetPosition().x +*/ (sinf(XMConvertToRadians(objTurret->GetRotation().y)) * 12.5f)), 10.0f, (/*objTurret->GetPosition().z +*/ (cosf(XMConvertToRadians(objTurret->GetRotation().y)) * 12.5f)) });
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
		// レベルアップ
		// ↓敵一体を倒したときの処理に差し換えてください↓
		if (playerLevel < LevelMax)
		{
			for (int i = 0; i < 4; i++)
			{
				if (enemyArray[i]->enemyDefeated) // デバッグのみ
				{
					levelUp++;
					enemyArray[i]->enemyDefeated = 0;
				}
			}
		}

		if (levelUp > 4)
		{
			playerLevel++;
			levelUp = 0;
		}

		// レベル1
		if (playerLevel == 1)
		{
			playerSpeed = playerSpeedLevel1;
			playerBulletType = normalBullet;
		}
		// レベル2
		else if (playerLevel == 2)
		{
			playerSpeed = playerSpeedLevel2;
			playerBulletType = normalBullet;
		}
		// レベル3
		else if (playerLevel == 3)
		{
			playerSpeed = playerSpeedLevel3;
			playerBulletType = spreadBullet;
		}

		// レベルダウン
		if (playerLevel > LevelMin)
		{
			for (int i = 0; i < 4; i++)
			{
				playerLevel -= enemyArray[i]->levelDown;
			}
		}

		if (playerLevel > LevelMax)
		{
			playerLevel = LevelMax;
		}
		if (playerLevel < LevelMin)
		{
			playerLevel = LevelMin;
		}
#pragma endregion

#pragma region プレイヤーボム管理
		if (bombFlag == 0)
		{
			// 敵を倒すとゲージが増える
			// ↓敵一体を倒したときの処理に差し換えてください↓
			for (int i = 0; i < 4; i++)
			{
				if (enemyArray[i]->enemyDefeated)
				{
					playerBombGage++;
					enemyArray[i]->enemyDefeated = 0;
				}
			}

			BombGageHalf = playerBombGage / 2;

			// ゲージがマックスの時、ボムフラグをオンにする
			if (playerBombGage == BombGageMax)
			{
				bombFlag = 1;
			}
		}

		// ボムフラグがオンの時、ボムを発射
		else if (bombFlag == 1)
		{
			if (input->TriggerKey(DIK_B))
			{
				//
				// ボムを使用
				//
				audio->PlayWave("Bomb.wav", Volume_Title);

				bombFlag = 0;
				for (int i = 0; i < 4; i++)
				{
					enemyArray[i]->defeated = true;
					enemyArray[i]->enemyDefeated = 0;
					playerScoreValue += 50;
				}

				if (objRareEnemy->active == true)
				{
					objRareEnemy->defeated = true;
					playerScoreValue += 100;
				}

				for (int i = 0; i < 100; i++)
				{
					CreateBombParticles(rand() % 401 - 200.0f, rand() % 401 - 200.0f);
				}

				playerBombGage = 0;
			}
		}
#pragma endregion

		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			for (int i = 0; i < 4; i++)
			{
				if (circlecircleIntersect(bullet->GetPosition(), enemyArray[i]->GetPosition(), 3.0f, 3.0f) == true)
				{
					audio->PlayWave("Kill.wav", Volume_Title);
					enemyArray[i]->defeated = true;
					playerScoreValue += 50;
				}
			}

			if (circlecircleIntersect(bullet->GetPosition(), objRareEnemy->GetPosition(), 3.0f, 3.0f) == true && objRareEnemy->active == true)
			{
				objRareEnemy->defeated = true;
				audio->PlayWave("Heal.wav", Volume_Title);
				playerLife++;
				playerScoreValue += 100;
			}
		}
		for (std::unique_ptr<PlayerBullet_R>& bulletR : bullets_R)
		{
			for (int i = 0; i < 4; i++)
			{
				if (circlecircleIntersect(bulletR->GetPosition(), enemyArray[i]->GetPosition(), 3.0f, 3.0f) == true)
				{
					audio->PlayWave("Kill.wav", Volume_Title);
					enemyArray[i]->defeated = true;
					playerScoreValue += 50;
				}
			}

			if (circlecircleIntersect(bulletR->GetPosition(), objRareEnemy->GetPosition(), 3.0f, 3.0f) == true && objRareEnemy->active == true)
			{
				objRareEnemy->defeated = true;
				audio->PlayWave("Heal.wav", Volume_Title);
				playerLife++;
				playerScoreValue += 100;
			}
		}
		for (std::unique_ptr<PlayerBullet_L>& bulletL : bullets_L)
		{
			for (int i = 0; i < 4; i++)
			{
				if (circlecircleIntersect(bulletL->GetPosition(), enemyArray[i]->GetPosition(), 3.0f, 3.0f) == true)
				{
					audio->PlayWave("Kill.wav", Volume_Title);
					enemyArray[i]->defeated = true;
					playerScoreValue += 50;
				}
			}

			if (circlecircleIntersect(bulletL->GetPosition(), objRareEnemy->GetPosition(), 3.0f, 3.0f) == true && objRareEnemy->active == true)
			{
				objRareEnemy->defeated = true;
				audio->PlayWave("Heal.wav", Volume_Title);
				playerLife++;
				playerScoreValue += 100;
			}
		}

		// パーティクル生成
		//if (input->TriggerKey(DIK_RETURN)) // デバッグ全部の敵を殺す
		//{
		//	for (int i = 0; i < 4; i++)
		//	{
		//		CreateParticles(enemyArray[i]->GetPosition().x, enemyArray[i]->GetPosition().z);
		//	}
		//}

		if (playerScoreValue / 2000 > lastEValueUp && playerScoreValue != 0)
		{
			lastEValueUp = playerScoreValue / 2000;
			eSpeedUp = true;
		}

		for (int i = 0; i < 4; i++) // 敵がライフに命中するとパーティクルを作成します
		{
			if (enemyArray[i]->destruction)
			{
				CreateParticles(enemyArray[i]->particlePosition.x, enemyArray[i]->particlePosition.z);
			}

			if (enemyArray[i]->timer >= enemyArray[i]->timeToTarget)
			{
				audio->PlayWave("Damage.wav", Volume_Title);
			}

			if (eSpeedUp)
			{
				enemyArray[i]->rngMin -= 40;
				if (i == 3)
				{
					eSpeedUp = false;
				}
			}
		}

		if (objRareEnemy->destruction)
		{
			CreateParticles(objRareEnemy->particlePosition.x, objRareEnemy->particlePosition.z);
		}

		if (input->TriggerKey(DIK_R))
		{
			playerLife--;
		}
		else if (input->TriggerKey(DIK_T))
		{
			playerLife++;
		}

		for (int i = 0; i < 4; i++)
		{
			playerLife -= enemyArray[i]->damage;
		}

		if (playerLife < 1)
		{
			playerLife = 0;
			if (playerScoreValue > sessionHighScoreValue)
			{
				sessionHighScoreValue = playerScoreValue;
			}
			audio->StopWave("GameScene.wav");
			audio->PlayWave("Result.wav", Volume_Title, true);
			gamePlayScene++;
			break;
		}
		if (playerLife > 8)
		{
			playerLife = 8;
		}

		if (lifeRotation.x > 45.0f)
		{
			rUpDown = false;
		}
		else if (lifeRotation.x < -45.0f)
		{
			rUpDown = true;
		}
		if (rUpDown)
		{
			lifeRotation.x += 0.4f;
		}
		else
		{
			lifeRotation.x -= 0.4f;
		}
		lifeRotation.y += 2.5f;
		if (lifeRotation.z > 45.0f)
		{
			rLeftRight = false;
		}
		else if (lifeRotation.z < -45.0f)
		{
			rLeftRight = true;
		}
		if (rLeftRight)
		{
			lifeRotation.z += 0.4f;
		}
		else
		{
			lifeRotation.z -= 0.4f;
		}
		objLife->SetRotation(lifeRotation);

#pragma region アップデート
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
		objRareEnemy->Update();
#pragma endregion

#pragma region デバックテキスト
		/*std::ostringstream playerLifeNumber;
		playerLifeNumber
			<< std::fixed << std::setprecision(0)
			<< playerLife;
		debugText->Print(playerLifeNumber.str(), 1150.0f, 45.0f, 5.0f);*/

		playerScore << "SCORE "
			<< std::fixed << std::setprecision(0)
			<< std::setw(7) << std::setfill('0') << playerScoreValue;
		debugText->Print(playerScore.str(), 20.0f, 20.0f, 5.0f);
#pragma endregion

		//Playerbullet->SetPosition({ objTurret->GetPosition().x, objTurret->GetPosition().y, objTurret->GetPosition().z });
		//Playerbullet->Update();

		//弾処理
		//デスフラグが立った弾を削除
		bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
			{
				return bullet->DeathGetter();
			});

		bullets_R.remove_if([](std::unique_ptr<PlayerBullet_R>& bullet)
			{
				return bullet->DeathGetter();
			});

		bullets_L.remove_if([](std::unique_ptr<PlayerBullet_L>& bullet)
			{
				return bullet->DeathGetter();
			});

		Attack();

		if (playerLevel > 2)
		{
			//拡散弾
			DiffusionAttack_R();
			DiffusionAttack_L();
		}

		//弾更新
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->Update();
		}

		for (std::unique_ptr<PlayerBullet_R>& bullet : bullets_R)
		{
			bullet->Update();
		}

		for (std::unique_ptr<PlayerBullet_L>& bullet : bullets_L)
		{
			bullet->Update();
		}

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
		break;

	case 2:
		if (input->TriggerKey(DIK_SPACE))
		{
			audio->StopWave("Result.wav");
			audio->PlayWave("Select.wav", Volume_Title);
			audio->PlayWave("Title.wav", Volume_Title, true);
			GameReset();
			gamePlayScene = 0;
		}

		playerScore << "SCORE "
			<< std::fixed << std::setprecision(0)
			<< std::setw(7) << std::setfill('0') << playerScoreValue;
		debugText->Print(playerScore.str(), 340.0f, 360.0f, 5.0f);

		hiScore << "HIGH SCORE "
			<< std::fixed << std::setprecision(0)
			<< std::setw(7) << std::setfill('0') << sessionHighScoreValue;
		debugText->Print(hiScore.str(), 235.0f, 260.0f, 5.0f);

		particleMan->Update();
		break;
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	switch (gamePlayScene)
	{
	case 0:
		switch (pageNo)
		{
		case 0:
			titleEndingSprite[0]->Draw();
			break;
		case 1:
			break;
		case 2:
			titleEndingSprite[2]->Draw();
			break;
		case 3:
			titleEndingSprite[3]->Draw();
			break;
		}
		break;
	case 1:
		spriteBG->Draw();
		break;
	case 2:
		titleEndingSprite[1]->Draw();
		break;
	}

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	Object3d::PreDraw(cmdList);
	Enemy::PreDraw(cmdList);
	RareEnemy::PreDraw(cmdList);
	PlayerBullet::PreDraw(cmdList);
	PlayerBullet_R::PreDraw(cmdList);
	PlayerBullet_L::PreDraw(cmdList);

	switch (gamePlayScene)
	{
	case 0:
		break;
	case 1:
		// 3D Object Drawing
	//objSkydome->Draw();
	//objGround->Draw();
		objTurret->Draw();
		objLife->Draw();

		//Bullet->SetPosition({ objTurret->GetPosition().x, objTurret->GetPosition().y, objTurret->GetPosition().z });
		//Bullet->Draw();

		//弾描画
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			bullet->Draw();
		}

		for (std::unique_ptr<PlayerBullet_R>& bullet : bullets_R)
		{
			bullet->Draw();
		}

		for (std::unique_ptr<PlayerBullet_L>& bullet : bullets_L)
		{
			bullet->Draw();
		}

		objRareEnemy->Draw();

		for (int i = 0; i < 4; i++)
		{
			enemyArray[i]->Draw();
		}

		// パーティクルの描画
		particleMan->Draw(cmdList);
		break;
	case 2:
		break;
	}

	PlayerBullet_L::PostDraw();
	PlayerBullet_R::PostDraw();
	PlayerBullet::PostDraw();
	Object3d::PostDraw();
	Enemy::PostDraw();
	RareEnemy::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	switch (gamePlayScene)
	{
	case 0:
		break;
	case 1:
		/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
		for (int i = 0; i < playerLife + 1; i++)
		{
			lifeGauge[i]->Draw();
		}

		switch (playerBombGage)
		{
		case 0:
		case 1:
			bombGauge[0]->Draw();
			break;
		case 2:
		case 3:
			bombGauge[1]->Draw();
			break;
		case 4:
		case 5:
			bombGauge[2]->Draw();
			break;
		case 6:
		case 7:
			bombGauge[3]->Draw();
			break;
		case 8:
		case 9:
			bombGauge[4]->Draw();
			break;
		case 10:
		case 11:
			bombGauge[5]->Draw();
			break;
		case 12:
		case 13:
			bombGauge[6]->Draw();
			break;
		case 14:
		case 15:
			bombGauge[7]->Draw();
			break;
		case 16:
		case 17:
			bombGauge[8]->Draw();
			break;
		case 18:
		case 19:
			bombGauge[9]->Draw();
			break;
		case 20:
		case 21:
			bombGauge[10]->Draw();
			break;
		case 22:
		case 23:
			bombGauge[11]->Draw();
			break;
		case 24:
		case 25:
			bombGauge[12]->Draw();
			break;
		case 26:
		case 27:
			bombGauge[13]->Draw();
			break;
		case 28:
		case 29:
			bombGauge[14]->Draw();
			break;
		case 30:
			bombGauge[15]->Draw();
			break;
		}

		bombSprite->Draw();

		switch (playerLevel)
		{
		case 1:
			playerLevelSprite[0]->Draw();
			break;
		case 2:
			playerLevelSprite[1]->Draw();
			break;
		case 3:
			playerLevelSprite[2]->Draw();
			break;
		}
		break;
	case 2:
		break;
	}

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
		particleMan->Add(120, pos, vel, acc, 3.0f, 0.0f);
	}
}

void GameScene::CreateBombParticles(float x, float z)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 20.0f;
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
		particleMan->Add(180, pos, vel, acc, 12.0f, 0.0f);
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

int GameScene::circlecircleIntersect(XMFLOAT3 c1, XMFLOAT3 c2, float r1, float r2)
{
	float d = sqrtf((c1.x - c2.x) * (c1.x - c2.x) + (c1.z - c2.z) * (c1.z - c2.z));

	if (d <= r1 - r2 || d <= r2 - r1 || d < r1 + r2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameScene::Attack()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		audio->PlayWave("Shot.wav", Volume_Title);

		//弾の速度
		const float Speed = 3.0f;
		XMVECTOR velocity = { 0, Speed, 0 };

		//速度ベクトルに合わせて回転させる
		velocity = XMVector3Transform(velocity, objTurret->GetMatWorld());

		/// <summary>
		/// 弾生成と初期化
		/// </summary>
		newBullet = PlayerBullet::Create(Bullet_Model, camera, { objTurret->GetPosition().x, objTurret->GetPosition().y, objTurret->GetPosition().z }, { objTurret->GetRotation().x, objTurret->GetRotation().y, objTurret->GetRotation().z }, velocity);
		//newBullet->PlayerBullet::Create(Bullet_Model, camera, { objTurret->GetPosition().x, objTurret->GetPosition().y, objTurret->GetPosition().z });
		//newBullet->PlayerBullet::Create(Bullet_Model,camera, TurretPos);

		//弾を登録
		bullets_.push_back(std::move(newBullet));
	}
}

void GameScene::DiffusionAttack_R()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//弾の速度
		const float Speed = 3.0f;
		XMVECTOR velocity = { -Speed, Speed, 0 };

		//速度ベクトルに合わせて回転させる
		velocity = XMVector3Transform(velocity, objTurret->GetMatWorld());

		/// <summary>
		/// 弾生成と初期化
		/// </summary>
		newBulletR = PlayerBullet_R::Create(Bullet_Model, camera, { objTurret->GetPosition().x, objTurret->GetPosition().y, objTurret->GetPosition().z }, { objTurret->GetRotation().x, objTurret->GetRotation().y, objTurret->GetRotation().z }, velocity);

		//弾を登録
		bullets_R.push_back(std::move(newBulletR));
	}
}

void GameScene::DiffusionAttack_L()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//弾の速度
		const float Speed = 3.0f;
		XMVECTOR velocity = { Speed, Speed, 0 };

		//速度ベクトルに合わせて回転させる
		velocity = XMVector3Transform(velocity, objTurret->GetMatWorld());

		/// <summary>
		/// 弾生成と初期化
		/// </summary>
		newBulletL = PlayerBullet_L::Create(Bullet_Model, camera, { objTurret->GetPosition().x, objTurret->GetPosition().y, objTurret->GetPosition().z }, { objTurret->GetRotation().x, objTurret->GetRotation().y, objTurret->GetRotation().z }, velocity);

		//弾を登録
		bullets_L.push_back(std::move(newBulletL));
	}
}

void GameScene::GameReset()
{
	pageNo = 0;
	playerLevel = 1;
	playerBombGage = 0;
	bombFlag = 0;
	playerLife = 8;
	TurretPos = { 0.0f, 0.0f, 0.0f };
	objTurret->SetRotation({ 0.0f, 0.0f, 0.0f });
	playerScoreValue = 0;
	for (int i = 0; i < 4; i++)
	{
		int RNG = rand() % 1;
		int RNG2 = rand() % 1;
		if (RNG == 1)
		{
			if (RNG2 == 1)
			{
				enemyArray[i]->SetPosition({ rand() % 101 - 150.0f, 10.0f, rand() % 101 - 150.0f });
			}
			else
			{
				enemyArray[i]->SetPosition({ rand() % 101 + 50.0f, 10.0f, rand() % 101 - 150.0f });
			}
		}
		else
		{
			if (RNG2 == 1)
			{
				enemyArray[i]->SetPosition({ rand() % 101 + 50.0f, 10.0f, rand() % 101 - 150.0f });
			}
			else
			{
				enemyArray[i]->SetPosition({ rand() % 101 + 50.0f, 10.0f, rand() % 101 + 50.0f });
			}
		}

		enemyArray[i]->defeated = false;
		enemyArray[i]->damage = 0;
		enemyArray[i]->destruction = false;
		enemyArray[i]->levelDown = 0;
		enemyArray[i]->movementFlag = false;
		enemyArray[i]->timer = 0;
		enemyArray[i]->rngMin = 1080;
	}
}