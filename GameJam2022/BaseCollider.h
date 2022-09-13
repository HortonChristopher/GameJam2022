#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "Enemy.h"
#include "RareEnemy.h"
#include "PlayerBullet.h"
#include "PlayerBullet_R.h"
#include "PlayerBullet_L.h"
#include "CollisionInfo.h"

/// <summary>
/// �R���C�_�[���N���X Collider base class
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;

	// ���z�f�X�g���N�^ Virtual destructor
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d* object) {
		this->object3d = object; }

	inline void SetObjectEnemy(Enemy* object) {
		this->enemy = object;
	}

	inline void SetObjectRareEnemy(RareEnemy* object) {
		this->rareEnemy = object;
	}

	inline void SetBullet(PlayerBullet* object) {
		this->playerBullet = object;
	}

	inline void SetBullet(PlayerBullet_R* object) {
		this->playerBullet_R = object;
	}

	inline void SetBullet(PlayerBullet_L* object) {
		this->playerBullet_L = object;
	}

	inline Object3d* GetObject3d() {
		return object3d;
	}

	inline Enemy* GetEnemy() {
		return enemy;
	}

	inline PlayerBullet* GetBullet() {
		return playerBullet;
	}

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐� Collision callback function
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

	/// <summary>
	/// �X�V update
	/// </summary>
	virtual void Update() = 0;
	// �`��^�C�v�擾 Shape type acquisition
	inline CollisionShapeType GetShapeType() { return shapeType; }

	inline void SetAttribute(unsigned short attribute) {
		this->attribute = attribute;
	}

	inline void AddAttribute(unsigned short attribute) {
		this->attribute |= attribute;
	}

	inline void RemoveAttribute(unsigned short attribute) {
		this->attribute &= !attribute;
	}

protected:
	Object3d* object3d = nullptr;
	Enemy* enemy = nullptr;
	RareEnemy* rareEnemy = nullptr;
	PlayerBullet* playerBullet = nullptr;
	PlayerBullet_R* playerBullet_R = nullptr;
	PlayerBullet_L* playerBullet_L = nullptr;
	// �`��^�C�v Shape type
	CollisionShapeType shapeType = SHAPE_UNKNOWN;

	unsigned short attribute = 0b1111111111111111;
};