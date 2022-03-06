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

		// ���X�g
		std::vector< GameObject*> m_objlist;
		std::vector< float> m_degreelist;
		//std::vector<std::pair<GameObject*, float>> m_lengthlist;
		std::vector<std::pair<float,GameObject*>> m_lengthlist;

		GameObject* player;

		// �ϐ�
		int _num = 0;
		int m_startlivrcnt = 0;		// �ŏ��ɂ��Ă��鐔
		int m_liveCount = 0;	    // �������Ă���G�̐�
		int m_deadcount = 0;		// �|������
		float m_enelistsize;		// ���X�g�̃T�C�Y
		const int m_lengthmax = 700;// �����ő�l

		bool m_deadFlg=false;

		bool m_button = false;
		bool m_inputflg = false;

		const float m_maxdis = 10000;	// �Ώ۔͈�
		GameObject* m_target;			// �^�[�Q�b�g
		int m_nearcnt;					// ���݂̉��Ԗڂ̋߂���\�����̃J�E���g
		const int m_maxnearcnt = 3;		// ���Ԗڂ��̍ő�l

		// �֐�
		void EnemyUpdate();
		void SerchDegree(GameObject* _obj);		// �p�x�擾�֐�
		void EnemyCameraLock();					// �J�����̃^�[�Q�b�g�擾�֐�
		void CompareLiveCnt();					// �������̔�r

		void DeadFlgCheck();

	public:
		EnemyManager(GameObject* _obj);
		~EnemyManager();

		void Start();
		void Update();

		void SetList(std::vector<Vector3*> _pos);
		std::vector< GameObject*> GetObjectList() { return m_objlist; }	// �G�l�~�[���X�g�擾
		GameObject* GetEnemy(int _num) { return  m_objlist[_num]; }		// �G�l�~�[�̎擾

		std::vector< float> GetDegreeList() { return m_degreelist; }	// �p�x���X�g�擾

		float GetLength(int _num) { return m_lengthlist[_num].first; }		// �������X�g�擾
		
		int GetLiveCount() { return m_liveCount; }						// �������擾	
		void LiveCountReduce() { m_liveCount--; };						// �������̌���					// ���������炷
	
		int GetDeadEnemyCnt() { return m_deadcount; }					// �|������

		bool GetDeadFlg() { return m_deadFlg; }
		void ResetDeadFlg() { m_deadFlg = false; }

		GameObject* GetLookObj() { return m_target; }
	};

}