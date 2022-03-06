#pragma once
#include "../../Component\GameObject.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/Quad2DMultiRenderComponent.h"
#include "../../DX11System\dx11mathutil.h"
#include "../Enemy/EnemyManager.h"
#include "MiniMap\MiniMapIconUI.h"

namespace Component {

	class MiniMapUI : public ComponentBase
	{
	private:
		// �R���|�\�l���g
		Transform* tra;
		Component::QuadRender* ren;
		Component::Quad2DRender* ren2d;

		// �ϐ�
		GameObject* m_camera;
		GameObject* player;
		Component::EnemyManager* enemana;

		Vector3 center;			// �~�j�}�b�v�̒��S�_
		Vector3 m_pos = Vector3(75, -648, 1);

		// ���X�g
		std::vector< GameObject*> miniiconlist;	// �A�C�R��

		void PosUpdate();		// �ꏊ�̍X�V
		void RoundCheck();		// �p�x����ʒu�w��


		bool init = false;
	public:
		MiniMapUI(GameObject* _obj);
		~MiniMapUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }// �ʒu�̐ݒ�
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }				// qt�̐ݒ�
	};
}