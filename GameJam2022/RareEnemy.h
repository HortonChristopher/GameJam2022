#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Model.h"
#include "Camera.h"
#include "CollisionInfo.h"
#include "input/Input.h"
#include "DirectXCommon.h"
#include "ParticleManager.h"

class BaseCollider;

// 3D�I�u�W�F�N�g 3D object
class RareEnemy
{
private: // �G�C���A�X alias
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X	 Subclass

	// �p�C�v���C���Z�b�g Pipeline set
	struct PipelineSet
	{
		// ���[�g�V�O�l�`�� Route signature
		ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g Pipeline state object
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// �萔�o�b�t�@�p�f�[�^�\����B0 Data structure for constant buffer B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// �R�c�ϊ��s�� 3D transformation matrix
	};

private: // �萔 constant


public: // �ÓI�����o�֐� Static member function
	// �ÓI������ Static initialization
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);

	// �O���t�B�b�N�p�C�v���C���̐��� Generate graphic pipeline
	static void CreateGraphicsPipeline();

	// �J�����̃Z�b�g Camera set
	static void SetCamera(Camera* camera) {
		RareEnemy::camera = camera;
	}

	// �`��O���� Pre-drawing processing
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// �`��㏈�� Post-drawing processing
	static void PostDraw();

	// 3D�I�u�W�F�N�g���� 3D object generation
	static RareEnemy* Create(Model* model = nullptr, DirectXCommon* dxCommon = nullptr);


private: // �ÓI�����o�ϐ� Static member variables
	// �f�o�C�X device
	static ID3D12Device* device;
	static ID3D12Device* device2;
	// �R�}���h���X�g Command list
	static ID3D12GraphicsCommandList* cmdList;
	static ID3D12GraphicsCommandList* cmdList2;
	// �e�N�X�`������p�p�C�v���C�� Pipeline for texture
	static PipelineSet pipelineSet;
	// �J���� camera
	static Camera* camera;

private:// �ÓI�����o�֐� Static member function

public: // �����o�֐� Member function
	/// <summary>
	/// �R���X�g���N�^ Constructor
	/// </summary>
	RareEnemy() = default;

	/// <summary>
	/// �f�X�g���N�^ Destructor
	/// </summary>
	virtual ~RareEnemy();

	/// <summary>
	/// ������ Initialize
	/// </summary>
	/// <returns></returns>
	//bool Initialize();
	virtual bool Initialize(DirectXCommon* dxCommon);

	// ���t���[������ Every frame processing
	//void Update();
	virtual void Update();

	// �`�� drawing
	//void Draw();
	virtual void Draw();

	/// <summary>
	/// ���[���h�s��̎擾 Get the world matrix
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// �R���C�_�[�̃Z�b�g Collider Set
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐� Collision callback function
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {}

	// ���W�̎擾 Get coordinates
	const XMFLOAT3& GetPosition() { return position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	static void SetDevice(ID3D12Device* device) { RareEnemy::device = device; }

	// ���W�̐ݒ� Coordinate setting
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	// �p�x�̐ݒ� Angle setting
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	// �X�P�[���̐ݒ� Scale setting
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	// ���f���̃Z�b�g Set of models
	void SetModel(Model* model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	/// <summary>
	/// ���f�����擾 Get the model
	/// </summary>
	/// <returns>���f��</returns>
	inline Model* GetModel() { return model; }

	void UpdateWorldMatrix();

	// �p�[�e�B�N������
	void CreateParticles(float x, float z);

	//private: // �����o�ϐ� Member variables
protected: // �����o�ϐ� Member variables

	// �N���X���i�f�o�b�O�p Class name (for debugging)
	const char* name = nullptr;

	// �R���C�_�[ Collider
	BaseCollider* collider = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	ParticleManager* particleMan = nullptr;

	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@ Constant buffer
	// �F colour
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[�� Local scale
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p Local rotation angle around the X, Y, Z axes
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W Local coordinates
	XMFLOAT3 position = { 0,10.0f,0 };
	// ���[�J�����[���h�ϊ��s�� Local world transformation matrix
	XMMATRIX matWorld;
	XMMATRIX matWorldE;
	// �e�I�u�W�F�N�g Parent object
	RareEnemy* parent = nullptr;
	// ���f�� model
	Model* model = nullptr;
	// �r���{�[�h Billboard
	bool isBillboard = false;

	int destructionTimer = 0;

	float x = 0.0f;
	float z = 0.0f;
	int timer = 0;
	float timeToTarget = 0.0f;
	bool movementFlag = false;
	int RNG = 0;

public:

	bool destruction = false;
	XMFLOAT3 particlePosition = { 0.0f, 10.0f, 0.0f };
	int enemyDefeated = 0; // �f�o�b�O�̂�
	int spawnTimer = 0;
};