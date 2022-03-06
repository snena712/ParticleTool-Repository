#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  FadeManager :public ComponentBase
	{
	private:
		enum class FadeState 
		{
			NONE,
			FADEIN,
			FADEOUT
		};
		GameObject* m_camera;

		Transform* tra;
		Component::QuadRender* ren;

		FadeState m_state = FadeState::NONE;

		float m_alpha = 1;

		int m_fadecnt = 0;
		std::string m_scenename;

		void PosUpdate();	
		
		// fadeの更新
		bool StartFade();
		bool EndFade();
	public:
		FadeManager(GameObject* _obj);
		~FadeManager();

		void Start();
		void Update();
		void DrawUI();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		// フェードのセット
		void SetFadeScene(float _cnt, std::string _name) { 
			m_fadecnt = _cnt;
			m_scenename = _name;
			m_state = FadeState::FADEIN; 
			m_alpha = 0; 
			owner->SetExistState(true);
		}
		void SetOutScene(float _cnt) {
			m_fadecnt = _cnt; 
			m_state = FadeState::FADEOUT; 
			m_alpha = 1; 
			owner->SetExistState(true);
		}
		
		bool GetFadeOut() { 
			if (m_alpha == 0)return true; else return false; }
	};
}
