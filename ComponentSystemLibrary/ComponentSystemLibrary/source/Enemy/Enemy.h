#pragma once
#include "../../Component\ModelComponent.h"
#include "../../Component\SphereColliderComponent.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component\RigidbodyComponent.h"
#include "../Compute/EffectManager.h"
#include "../UI/Enemy/EnemyPosFrame.h"
#include "../../Component/SceneManager.h"
#include "../Smog/SmogManager.h"
#include "../Effect/Bomb/BombAnim.h"
#include <random>

namespace Component {
	class Enemy :public ComponentBase
	{
	public:
		enum class MoveDir
		{
			UP,
			DOWN,
			RIGHT,
			LEFT,
			STRREAT,
			RSPIN,
			LSPIN,
			SLOW,
			STOP
		};

		enum Status
		{
			LIVE,
			DEAD
		};
		enum class Action
		{
			Move
		};

	private:
		MoveDir movedir;
		Status status;
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::QuadRender* ren;
		Component::SphereCollider* col;
		Component::SmogManager* smogmana;
		EffectManager* efemana;

		// �̗�
		float life = 20;			// ���C�t
		float oldlife = life;		// �O�t���[���̃��C�t

		//-----���S�p------//
		int m_nowdeadcnt = 0;
		const int m_maxdeadcnt = 60;

		//---���b�N�I���֘A---//
		bool lockonFlg = false;		// ���b�N�I���̏��
		int m_lockoncnt = 0;

		// ��O�ɍs�������̃t���O
		bool m_outside=false;
		const int m_length = 500;

		Component::EnemyPosFrame *frame;			// �g
		GameObject *bomb;			// �g

		//-----�ړ�----
		const float speed = 0.01;			// �ړ����x
		const float Lside = 3;			// ���X�e�B�b�N��X�̈ړ�����
		const float m_maxSpeed = 2.5;	// �ړ��ő�l
		const float m_minSpeed = 1.5f;	// �ړ��ŏ��l

		//-----�ړ�----//
		int m_movecnt = 60;				// �ړ��p�̃J�E���g
		int m_nowmovecnt;				// ���݂̃J�E���g
		//const float speed = 0.00;			// �ړ����x
		//const float Lside = 0;				// ���X�e�B�b�N��X�̈ړ�����
		//const float m_maxSpeed = 0;			// �ړ��ő�l
		//const float m_minSpeed = 0.00f;		// �ړ��ŏ��l

		float straightSpeed = m_maxSpeed / 2;

		// ���̃J�E���g
		int m_nowsmogcnt = 0;
		int m_maxsmogcnt = 1;

		void MoveUpdate();				// �ړ��ʂ̌v�Z
		void AutoGo(XMFLOAT4 _axisZ);	// �ړ�
		void Collision();				// �����蔻��
		void StatusUpdate();			// ��ԕύX
		void RandMove();				// ��Ԃ������_���ɕύX
		void LifeCheck();				// �̗͂̊m�F
		void DeadUpdate();				// ���S����
		void LockOnUpdate();			// ���b�N�I���̍X�V
		void DeadFly(XMFLOAT4 _axisZ);	// ���S���̔�s
		void SmogFly();					// ��s���̉_

	public:
		Enemy(GameObject *_owner);
		~Enemy();

		void Start();
		void Update();
		void DrawObject();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }

		bool GetStatus() { if (status == LIVE) { return true; } else { return false; } }

		void LowerLife(int _num) { life -= _num; }
		void SetLoclOnFlg(bool _flg) { lockonFlg = _flg; }

		void SetLockOnCnt(int _cnt) { m_lockoncnt = _cnt; }
		
	};

}