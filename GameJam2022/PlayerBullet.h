#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "CollisionInfo.h"

class BaseCollider;

class PlayerBullet
{
private: // �G�C���A�X alias
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

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
		PlayerBullet::camera = camera;
	}

	// �`��O���� Pre-drawing processing
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// �`��㏈�� Post-drawing processing
	static void PostDraw();

	// 3D�I�u�W�F�N�g���� 3D object generation
	static std::unique_ptr<PlayerBullet>Create(Model* model,Camera* camera, XMFLOAT3 pos, XMFLOAT3 rot, const XMVECTOR& velocity);


private: // �ÓI�����o�ϐ� Static member variables
	// �f�o�C�X device
	static ID3D12Device* device;
	// �R�}���h���X�g Command list
	static ID3D12GraphicsCommandList* cmdList;
	// �e�N�X�`������p�p�C�v���C�� Pipeline for texture
	static PipelineSet pipelineSet;
	// �J���� camera
	static Camera* camera;
	//���x
	static XMVECTOR velocity_;

	//����
	static const int LifeTime = 60 * 3;

private:// �ÓI�����o�֐� Static member function

public: // �����o�֐� Member function
	/// <summary>
	/// �R���X�g���N�^ Constructor
	/// </summary>
	PlayerBullet() = default;

	/// <summary>
	/// �f�X�g���N�^ Destructor
	/// </summary>
	virtual ~PlayerBullet();

	/// <summary>
	/// ������ Initialize
	/// </summary>
	/// <returns></returns>
	//bool Initialize();
	virtual bool Initialize(XMFLOAT3 pos, XMFLOAT3 rot);

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
	void SetCollider(BaseCollider * collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐� Collision callback function
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo & info) {}

	// ���W�̎擾 Get coordinates
	const XMFLOAT3& GetPosition() { return position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	// ���W�̐ݒ� Coordinate setting
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	// �p�x�̐ݒ� Angle setting
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	// �X�P�[���̐ݒ� Scale setting
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	// ���f���̃Z�b�g Set of models
	void SetModel(Model * model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	/// <summary>
	/// ���f�����擾 Get the model
	/// </summary>
	/// <returns>���f��</returns>
	inline Model* GetModel() { return model; }

	void UpdateWorldMatrix();

	//�f�X�t���O�Q�b�^�[
	bool DeathGetter()const { return DeathFlag; }


	//private: // �����o�ϐ� Member variables
protected: // �����o�ϐ� Member variables

	// �N���X���i�f�o�b�O�p Class name (for debugging)
	const char* name = nullptr;

	// �R���C�_�[ Collider
	BaseCollider* collider = nullptr;

	//�C���v�b�g
	Input* input = nullptr;

	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@ Constant buffer
	// �F colour
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[�� Local scale
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 scale2 = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p Local rotation angle around the X, Y, Z axes
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 rotationE = { 0,0,0 };
	// ���[�J�����W Local coordinates
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 positionE = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s�� Local world transformation matrix
	XMMATRIX matWorld;
	XMMATRIX matWorldE;
	// �e�I�u�W�F�N�g Parent object
	PlayerBullet* parent = nullptr;
	// ���f�� model
	Model* model = nullptr;
	// �r���{�[�h Billboard
	bool isBillboard = false;

	float velocityVx = 0.0f;
	float velocityVy = 0.0f;
	float velocityVz = 0.0f;

	XMVECTOR velocityV;
	bool velocitySet = false;

	//�f�X�^�C��
	int DeathTime = LifeTime;

	//�f�X�t���O
	bool DeathFlag = false;
};

