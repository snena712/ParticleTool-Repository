#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/CameraComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../StartUIColorComponent.h"

namespace Component {
	class EnemyPosFrame :public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::Camera* cameraCompo;

		Transform* tra;
		Component::Quad2DRender* ren;

		void PosUpdate();

		GameObject *Ene;		// �G�̈ʒu

		XMVECTOR Screenpos(XMVECTOR World_Pos);

		bool m_drawflg = false;
	public:
		EnemyPosFrame(GameObject* _obj);
		~EnemyPosFrame();

		void Start();
		void Update();
		void DrawObject();

		void SetParent(GameObject* _obj) { Ene = _obj; }	// �Q�[���I�u�W�F�N�g�̒��g�n��
		bool GetDrawFlg() { return m_drawflg; }
		void CheckScreen();
	};
}
