#pragma once
#include <vector>
#include "../../Component\GameObject.h"
#include "../../DX11System\dx11mathutil.h"
#include "../ObjectShape/Cone.h"
#include "../F1_PlayerObj.h"
#include <random>

namespace Component {

	class JetFire : public ComponentBase
	{
	private:
		Transform* tra;

		Component::PlayerObj* player;
		std::vector<Cone*> conelist;

		int m_nowFlickcnt = 0;
		int m_maxFlickcnt = 1;
		float m_firesize = 0;

		const int conenum = 3;

		void FlickerFire();	// ‰Î‚Ì‚¿‚ç‚Â‚«‚ðƒ‰ƒ“ƒ_ƒ€‚És‚¤
	public:
		JetFire(GameObject* _obj);
		~JetFire();

		void Start();
		void Update();
		void DrawObject();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }
	};
}
