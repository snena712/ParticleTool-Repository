#pragma once
#include "IRenderBase.h"
#include "../DX11System/PostEffectQuad.h"
#include "../source/Compute/SomeSupportCompute.h"
#include "../source/PostEffect/RenderTarget2D.h"

struct TransDate
{
	Vector3 pos;
	Vector2 size;
};

namespace Component {
	class PostEffectComponent :public IRenderBase {

	public:
		PostEffectComponent(GameObject* Owner, const char* _firstfilename, const char* _secondfilename, std::string _vs, std::string _ps);
		~PostEffectComponent();
		
		//----------画像サイズの設定-----------//
		float GetWidth() { return width; }								
		void SetWidth(float _width) { width = _width; }
		float GetHeight() { return height; }
		void SetHeight(float _height) { height = _height; }

		void SetPostID(int _id) { m_drawID = _id; }  // 描画番号の設定
		int GetPostID() { return m_drawID; }		 // 描画番号を渡す

		//-----------テクスチャの変更-----------//	
		std::string GetTexFileName() { return texFileName; }			// テクスチャ名の取得
		std::string* GetTexFileNameAdress() { return &texFileName; }	// アドレス渡し（基本使わない）

		//-----------描画-----------//
		void Start();
		void Update();
		void DrawObject();
		void NormalDraw();		// 通常の描画設定


		//---------UVの設定---------//
		void SetUVPos(DirectX::XMFLOAT2 left_top, DirectX::XMFLOAT2 right_top, DirectX::XMFLOAT2 left_bottom, DirectX::XMFLOAT2 right_bottom);


		//-----描画するエフェクトの個数と場所とサイズ----//
		void SetPosList(std::vector<Vector3> *_pos) { poslist =*_pos; }
		void SetSizeList(std::vector<Vector3*> _size) { sizelist = _size; }

	private:
		//====================directxの設定========================//
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_devContext;
		
		//=========================変数===========================//
		std::string texFileName;	
		std::string texSubFileName;		// ２つめ　<合成物>
		std::string m_vsmultistr = "shader/vsmultibiilbord.hlsl";
		std::string m_psmultistr = "shader/multips.hlsl";
		
		float width;				// 横の大きさ
		float height;				// 縦の大きさ
		int m_drowcnt;				// 描画する数
		DirectX::XMFLOAT2 uv[4];	// uvの位置
		DirectX::XMFLOAT4 color;	// 色
		
		static PostEffectQuad postquad;					// 四角形
		static unsigned int  postcounter;				// 描画の個数のカウント

		// ペアにする
		std::vector<Vector3> poslist;
		std::vector<Vector3*> sizelist;

		int m_drawID;		// 描画する順番(現状使ってない)
	};
}

//-----------------------------
// create by Suehara
//-----------------------------