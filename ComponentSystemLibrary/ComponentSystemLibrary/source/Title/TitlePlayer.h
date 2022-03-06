#pragma once
#include "../../Component/GameObject.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../DX11System/DX11util.h"
#include "../StageManagerClass.h"
#include "../BulletManager.h"
#include "../../source/ObjectShape/Cone.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/RandomClass.h"
#include "../../DX11System/mystring.h"

namespace Component {
	class TitlePlayer :public ComponentBase
	{
	private:
		//----コンポーネント------//
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		GameObject* fade;

		// 位置
		DirectX::XMFLOAT3 g_startpos;		// キー入力された際の開始位置

		// コントローラー
		int ControllerNum = -1;
		bool m_inputflg = false;		// 連続入力停止

		int m_inputcnt = 0;
		const int m_inputmax = 30;		// カウント

		const  XMFLOAT4 _base= XMFLOAT4(0.7f, 0.7f, 0.7f, 1);
		XMFLOAT4 m_col2= XMFLOAT4(0.7f, 0.7f, 0.7f, 1);
		//-------外部クラスの変数--------//

		//-------関数--------//

		void KeyBord();							// キーボード

		void Controller();
		void InputCnt();

		void String();
	public:
		TitlePlayer(GameObject* _obj);
		~TitlePlayer();

		//----基本関数----//
		void Start();
		void Update();
		void Input();
		void DrawUI();

		bool GetInputFlg() { return m_inputflg; }
	};
}