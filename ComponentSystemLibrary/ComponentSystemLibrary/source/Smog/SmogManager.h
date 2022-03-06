#pragma once
#include "../../Component/GameObject.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"
#include "SmogPlayer.h"
#include "EnemyWingSmog.h"
#include "WingSmog.h"

namespace Component {

	class SmogManager : public ComponentBase
	{

	private:
		Component::QuadRender* ren;
		Transform* targetTra;

		std::vector< SmogPlayer*> smoglist;		// �_�̃��X�g
		std::vector< WingSmog*> winglist;		// �_�̃��X�g
		std::vector< EnemyWingSmog*> ewinglist;		// �_�̃��X�g
		const int smognum = 60;
		const int wingnum = 20;
		const int enenum = 60;

		int enesmognum = 0;						// �G�̐��ɉ����Ēǉ�

	public:
		SmogManager(GameObject* _obj);
		~SmogManager();
		void Start();
		void Update();
		void SetActive(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);		// �����m�F
		void SetWingSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce);		// �H�̉_
		void SetEnemyDeadSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);	// �G�̎��S���̉_
		void SetEnemySmog(Vector3 _vec,XMFLOAT4 _qt, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);		// �G�̉H�_
	};

}