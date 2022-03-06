#pragma once
#include "../../../../Component/GameObject.h"
#include "../../../../Component/QuadRenderComponent.h"
#include "../../../../DX11System\dx11mathutil.h"
#include "../../../../Component/SceneManager.h"
#include "../../../../DX11System/mystring.h"
#include "../../../../DX11System/Time.h"
#include "../../../Enemy/EnemyManager.h"

namespace Component {
	class  ScoreUI :public ComponentBase
	{
	private:

		int m_score = 0;				// �Q�[���J�n������
		const float m_scorepoint = 1000;	// �G�̃X�R�A
		int m_plusscore = 0;			// �v���X�̃X�R�A

		int m_deadcnt = 0;				// ���݂̎��S��

		int m_nowscorecnt = 0;			// �v���X�̕\���b��
		const int m_maxscorecnt = 30;
		


		Component::EnemyManager* enemana;

		void PosUpdate();

		void String();
		void ScoreUpdate();		// �V�[���؊������ɃX�R�A�̍X�V���s��
	public:
		ScoreUI(GameObject* _obj);
		~ScoreUI();

		void Start();
		void Update();
		void DrawObject();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
