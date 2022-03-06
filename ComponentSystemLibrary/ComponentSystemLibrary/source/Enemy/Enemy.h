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

		// 体力
		float life = 20;			// ライフ
		float oldlife = life;		// 前フレームのライフ

		//-----死亡用------//
		int m_nowdeadcnt = 0;
		const int m_maxdeadcnt = 60;

		//---ロックオン関連---//
		bool lockonFlg = false;		// ロックオンの状態
		int m_lockoncnt = 0;

		// 場外に行った時のフラグ
		bool m_outside=false;
		const int m_length = 500;

		Component::EnemyPosFrame *frame;			// 枠
		GameObject *bomb;			// 枠

		//-----移動----
		const float speed = 0.01;			// 移動速度
		const float Lside = 3;			// 左スティックのXの移動制限
		const float m_maxSpeed = 2.5;	// 移動最大値
		const float m_minSpeed = 1.5f;	// 移動最小値

		//-----移動----//
		int m_movecnt = 60;				// 移動用のカウント
		int m_nowmovecnt;				// 現在のカウント
		//const float speed = 0.00;			// 移動速度
		//const float Lside = 0;				// 左スティックのXの移動制限
		//const float m_maxSpeed = 0;			// 移動最大値
		//const float m_minSpeed = 0.00f;		// 移動最小値

		float straightSpeed = m_maxSpeed / 2;

		// 煙のカウント
		int m_nowsmogcnt = 0;
		int m_maxsmogcnt = 1;

		void MoveUpdate();				// 移動量の計算
		void AutoGo(XMFLOAT4 _axisZ);	// 移動
		void Collision();				// 当たり判定
		void StatusUpdate();			// 状態変更
		void RandMove();				// 状態をランダムに変更
		void LifeCheck();				// 体力の確認
		void DeadUpdate();				// 死亡処理
		void LockOnUpdate();			// ロックオンの更新
		void DeadFly(XMFLOAT4 _axisZ);	// 死亡時の飛行
		void SmogFly();					// 飛行時の雲

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