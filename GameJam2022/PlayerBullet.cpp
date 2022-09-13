#include "PlayerBullet.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

// �ÓI�����o�ϐ��̎��� The entity of a static member variable
ID3D12Device* PlayerBullet::device = nullptr;
ID3D12GraphicsCommandList* PlayerBullet::cmdList = nullptr;
PlayerBullet::PipelineSet PlayerBullet::pipelineSet;
Camera* PlayerBullet::camera = nullptr;
XMVECTOR PlayerBullet::velocity_ = { 0, 0, 0 };

void PlayerBullet::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptr�`�F�b�N nullptr check
	assert(device);

	PlayerBullet::device = device;
	PlayerBullet::camera = camera;

	// �O���t�B�b�N�p�C�v���C���̐��� Generate graphic pipeline
	CreateGraphicsPipeline();

	// ���f���̐ÓI������ Static initialization of the model
	Model::StaticInitialize(device);

	
}

void PlayerBullet::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g Vertex shader object
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g Pixel shader object
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g Error object

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C�� Loading and compiling vertex shaders
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJVertexShader.hlsl",	// �V�F�[�_�t�@�C���� Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ��� Enable to include
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w�� Entry point name, shader model specification
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[ Copy the error content from errorBlob to string type
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\�� Display error details in output window 
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C�� Loading and compiling pixel shaders
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJPixelShader.hlsl",	// �V�F�[�_�t�@�C���� Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ��� Enable to include
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w�� Entry point name, shader model specification
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ� Debug settings
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[ Copy the error content from errorBlob to string type
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\�� Display error details in output window
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g Vertex layout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��) xy coordinates (it is easier to see if written in one line)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��) Normal vector (easier to see if written in one line)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��) uv coordinates (it is easier to see if written in one line)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ� Set the flow of the graphics pipeline
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N Sample mask
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g Rasterizer state
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g Depth stencil state
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ� Render target blend settings
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`�� RBGA Draw all channels
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ� Blend state setting
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g Depth buffer format
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ� Vertex layout settings
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j Shape shape setting (triangle)
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1�� One drawing target
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA RGBA specified from 0 to 255
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O Sampling once per pixel

	// �f�X�N���v�^�����W Descriptor range
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^ t0 register

	// ���[�g�p�����[�^ Route parameters
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[ Static sampler
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ� Route signature settings
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y Serialization of automatic version judgment
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐��� Route signature generation
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSet.rootsignature));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐��� Graphics pipeline generation
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

void PlayerBullet::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[ Error if PreDraw and PostDraw are not called in pairs
	assert(PlayerBullet::cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g Set command list
	PlayerBullet::cmdList = cmdList;

	// �v���~�e�B�u�`���ݒ� Set primitive shape
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void PlayerBullet::PostDraw()
{
	// �R�}���h���X�g������ Cancel command list
	PlayerBullet::cmdList = nullptr;
}

std::unique_ptr<PlayerBullet> PlayerBullet::Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 rot, const XMVECTOR& velocity)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐� Instantiate a 3D object
	PlayerBullet* playerBullet = new PlayerBullet();
	if (playerBullet == nullptr) {
		return nullptr;
	}

	// ������Initialization
	if (!playerBullet->Initialize(pos, rot)) {
		delete playerBullet;
		assert(0);
	}

	if (model) {
		playerBullet->SetModel(model);

	}

	//�J�����̃Z�b�g
	if (camera)
	{
		playerBullet->SetCamera(camera);
	}

	velocity_ = velocity;

	//float scale_val = 20;
	//object3d->scale = { scale_val,scale_val,scale_val };

	return std::unique_ptr<PlayerBullet>(playerBullet);
}

PlayerBullet::~PlayerBullet()
{
	if (collider) {
		// �R���W�����}�l�[�W������o�^���������� Unregister from Collision Manager
		CollisionManager::GetInstance()->RemoveCollider(collider);

		delete collider;
	}
}

bool PlayerBullet::Initialize(XMFLOAT3 pos, XMFLOAT3 rot)
{
	// nullptr�`�F�b�N nullptr check
	assert(device);

	position = pos;

	rotation = rot;

	HRESULT result;
	// �萔�o�b�t�@�̐��� Generate constant buffer
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\ Uploadable
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));

	// �N���X���̕�������擾 Get character examples of class name
	name = typeid(*this).name();

	return true;
}

void PlayerBullet::Update()
{
	// �����蔻��X�V Collision detection update
	if (collider) {
		collider->Update();
	}

	assert(camera);

	HRESULT result;

	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();

	// �萔�o�b�t�@�փf�[�^�]�� Data transfer to a constant buffer
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * matViewProjection;	// �s��̍��� Matrix composition
	constBuffB0->Unmap(0, nullptr);

	//�X�V����
	position.x += 3.0f;
	//position += velocity_;

	// �����蔻��X�V Collision detection update
	if (collider) {
		collider->Update();
	}
}

void PlayerBullet::Draw()
{
	// nullptr�`�F�b�N nullptr check
	assert(device);
	assert(PlayerBullet::cmdList);

	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ� Do not draw without model assignment
	if (model == nullptr) {
		return;
	}

	// �p�C�v���C���X�e�[�g�̐ݒ� Setting the pipeline state
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ� Route signature settings
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	// �萔�o�b�t�@�r���[���Z�b�g Set constant buffer view
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

	// ���f���`�� Model drawing
	model->Draw(cmdList);
}

void PlayerBullet::SetCollider(BaseCollider* collider)
{
	collider->SetBullet(this);
	this->collider = collider;

	// �R���W�����}�l�[�W���o�^ Collision manager registration
	CollisionManager::GetInstance()->AddCollider(collider);

	UpdateWorldMatrix();
	// �R���C�_�[���X�V���Ă��� Keep the collider updated
	collider->Update();
}

void PlayerBullet::UpdateWorldMatrix()
{
	assert(camera);

	XMMATRIX matScale, matRot, matTrans, matScale2, matRot2, matTrans2;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	/*matScale2 = XMMatrixScaling(scale2.x, scale2.y, scale2.z);
	matRot2 = XMMatrixIdentity();
	matRot2 *= XMMatrixRotationZ(XMConvertToRadians(rotationE.z));
	matRot2 *= XMMatrixRotationX(XMConvertToRadians(rotationE.x));
	matRot2 *= XMMatrixRotationY(XMConvertToRadians(rotationE.y));
	matTrans2 = XMMatrixTranslation(positionE.x, positionE.y, positionE.z);*/

	// ���[���h�s��̍��� World matrix composition
	if (isBillboard && camera) {
		const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

		//matWorld = XMMatrixIdentity();
		//matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
		//matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
		//matWorld *= matBillboard;
		//matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

		//matWorldE = XMMatrixIdentity();
		//matWorldE *= matScale2;
		//matWorldE *= matRot2;
		//matWorldE *= matBillboard;
		//matWorldE *= matTrans2;

		//matWorld *= matWorldE;

		matWorld = XMMatrixIdentity();
		//matWorld *= (matScale * matScale2);
		matWorld *= matScale;
		///matWorld *= (matRot * matRot2);
		matWorld *= matRot;
		matWorld *= matBillboard;
		//matWorld *= (matTrans * matTrans2);
		matWorld *= matTrans;
	}
	else {
		//matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
		//matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
		//matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
		//matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

		//matWorldE = XMMatrixIdentity();
		//matWorldE *= matScale2;
		//matWorldE *= matRot2;
		//matWorldE *= matTrans2;

		//matWorld *= matWorldE;

		matWorld = XMMatrixIdentity();
		//matWorld *= (matScale * matScale2);
		matWorld *= matScale;
		//matWorld *= (matRot * matRot2);
		matWorld *= matRot;
		//matWorld *= (matTrans * matTrans2);
		matWorld *= matTrans;
	}


	// �e�I�u�W�F�N�g������� If there is a parent object
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}
}

