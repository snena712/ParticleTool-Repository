#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  FadeComponent :public ComponentBase
	{
	private:
		enum class FadeState
		{
			NONE,
			FADEIN,
			FADEOUT
		};

		Component::QuadRender* ren;

		FadeState m_state = FadeState::NONE;

		float m_alpha = 1;
		float m_oldalpha = 1;

		int m_fadecnt = 0;


		// fadeの更新
		bool StartFade();
		bool EndFade();
	public:
		FadeComponent(GameObject* _obj);
		~FadeComponent();

		void Update();
		void DrawObject();

		// フェードのセット
		void SetFade(float _cnt) {
			m_fadecnt = _cnt;
			m_state = FadeState::FADEIN;
			m_alpha = 0;
			owner->SetExistState(true);
		}
		void SetOut(float _cnt) {
			m_fadecnt = _cnt;
			m_state = FadeState::FADEOUT;
			m_alpha = 1;
			owner->SetExistState(true);
		}

		bool GetFadeOut() {
			if (m_alpha == 0)return true; else return false;
		}
	};
}
