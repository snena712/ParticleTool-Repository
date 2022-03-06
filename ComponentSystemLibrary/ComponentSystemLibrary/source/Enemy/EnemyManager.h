#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "Enemy.h"
#include "../../Component/SceneManager.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/CDirectInput.h"
#include <utility>
#include "../MainGameManager.h"

namespace Component {
	class EnemyManager : public ComponentBase
	{
	private:

		// リスト
		std::vector< GameObject*> m_objlist;
		std::vector< float> m_degreelist;
		//std::vector<std::pair<GameObject*, float>> m_lengthlist;
		std::vector<std::pair<float,GameObject*>> m_lengthlist;

		GameObject* player;

		// 変数
		int _num = 0;
		int m_startlivrcnt = 0;		// 最初にいている数
		int m_liveCount = 0;	    // 生存している敵の数
		int m_deadcount = 0;		// 倒した数
		float m_enelistsize;		// リストのサイズ
		const int m_lengthmax = 700;// 距離最大値

		bool m_deadFlg=false;

		bool m_button = false;
		bool m_inputflg = false;

		const float m_maxdis = 10000;	// 対象範囲
		GameObject* m_target;			// ターゲット
		int m_nearcnt;					// 現在の何番目の近くを表すかのカウント
		const int m_maxnearcnt = 3;		// 何番目かの最大値

		// 関数
		void EnemyUpdate();
		void SerchDegree(GameObject* _obj);		// 角度取得関数
		void EnemyCameraLock();					// カメラのターゲット取得関数
		void CompareLiveCnt();					// 生存数の比較

		void DeadFlgCheck();

	public:
		EnemyManager(GameObject* _obj);
		~EnemyManager();

		void Start();
		void Update();

		void SetList(std::vector<Vector3*> _pos);
		std::vector< GameObject*> GetObjectList() { return m_objlist; }	// エネミーリスト取得
		GameObject* GetEnemy(int _num) { return  m_objlist[_num]; }		// エネミーの取得

		std::vector< float> GetDegreeList() { return m_degreelist; }	// 角度リスト取得

		float GetLength(int _num) { return m_lengthlist[_num].first; }		// 距離リスト取得
		
		int GetLiveCount() { return m_liveCount; }						// 生存数取得	
		void LiveCountReduce() { m_liveCount--; };						// 生存数の減少					// 生存数減らす
	
		int GetDeadEnemyCnt() { return m_deadcount; }					// 倒した数

		bool GetDeadFlg() { return m_deadFlg; }
		void ResetDeadFlg() { m_deadFlg = false; }

		GameObject* GetLookObj() { return m_target; }
	};

}