#pragma once
#include "../../Component/GameObject.h"
#include "../../Component\CameraComponent.h"
#include "../../source/StageManagerClass.h"
#include "../../source/GoalUIClass.h"
#include "../../source/BulletManager.h"
#include "../../DX11System/RandomClass.h"
#include "../../source/F1_PlayerObj.h"
#include "../../source/Enemy/EnemyManager.h"
#include "../../source/MainGameManager.h"
#include "../../Component/SceneManager.h"
#include "../../Component/Scene.h"
#include "../../source/GoalClass.h"
#include "../../DX11System/CDirectInput.h"
#include <array>

namespace Component {

	class SelectSceneCamera : public ComponentBase
	{
	public:
		enum class CameraType {
			ROTATE,
			STATIC
		};

	private:
		Transform* tra;
		Component::Camera* cam;
		Component::BulletManager* bul;
		Component::PlayerObj* playercompo;
		CameraType camtype = CameraType::ROTATE;	// �J�����̎��
		Component::EnemyManager* enemana;

		GameObject* player;
		StageManagerClass* stage;
		GoalUIClass* goalUi;

		Transform*  playertra;
		bool once = false;	// �S�[���p

		int ControllerNum = -1; //�R���g���[���[


		XMFLOAT3 pos;
		XMFLOAT3 g_startpos;
		int nowcnt = 0;	// ���݂̃J�E���g
		int g_nowrotcnt = 0;
		const int g_rotcnt = 45;	// �I���܂ł̃J�����ړ��J�E���g

		Vector3 m_pos;				// �ꏊ�̃��C��

		//---------�X�^�[�g�p---------//
		int m_startcnt = 0;
		float azimuth = 210;				// �J�n�p�x

		//--------�������p�ϐ�---------//
		int m_nowrandcnt = 0;
		const int m_maxrandcnt = 45;		// �J�����̂Ԃ�銴�o
		Vector3 m_randpos;					// �Ԃ�̈ʒu
		const float m_poslength = 23;		// �v���C���[�ƃJ�����̋����̏��



		void Input();

		void UpdateSphereCamera(
			float radius,					// ���a(����)
			float elevation,				// �p�i���́j
			float azimuth,					// ���ʊp�i���́j
			const DirectX::XMFLOAT3& lookat);		// �����_�i���́j

		void CameraPos();			// �J�����̈ʒu

		void FPSCamera();
		void TPSCamera();
		void DelayCamera();

		void TPSRotateCamera( float _pluscnt);		// �J�n�����o�p
		void ShakeCamera();								// �J�����̂Ԃ�
		void DeadCamera();								// ���S���p

	public:
		SelectSceneCamera(GameObject* _obj);
		~SelectSceneCamera();

		void CameraUpdate();

		void Update();
		void Start();
	};
}
