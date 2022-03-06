#pragma once
#include "../../Component/ModelComponent.h"

namespace Component {
	class  PlayerJatParts : public ComponentBase
	{
	private:

		GameObject* player;

		Transform* tra;
		Component::ModelRender* model;

		int m_rot;				// Œ»İ‚Ì•ÛŠp“x
		int m_maxrot = 10;		// Å‘å•ÛŠp“x
		int m_rotreturn = 1;	// –ß‚éŠp“x•
		bool m_rotflg = false;

		void PosUpdate();
		void RotUpdate();		// Šp“x‚ÌXV
	public:
		PlayerJatParts(GameObject* _obj);
		~PlayerJatParts();

		void Start();
		void Update();

		void SetRot(int _num);	// Šp“x‚Ìİ’è

	};
}

