#pragma once

#include "IRenderBase.h"
#include "../DX11System/quad2D.h"

namespace Component {
	class Quad2DRender :public IRenderBase {

	public:
		Quad2DRender(GameObject* _owner, const char* _filename);
		virtual ~Quad2DRender();

		virtual void Start();
		virtual void DrawUI();

#ifdef DEBUG
		virtual void ImGuiDebug();
#endif

		std::string GetTexFileName() { return texFileName; }				//テクスチャ名の取得
		std::string* GetTexFileNameAdress() { return &texFileName; }		//アドレス渡し（基本使わない）

		void ChangeTexture(const char* _fileName);							//動的なテクスチャ変更

		float GetWidth() { return width; }
		void SetWidth(float _width) { width = _width; }
		float GetHeight() { return height; }
		void SetHeight(float _height) { height = _height; }

		AnchorType GetAnchorType() { return anchorType; }
		void SetAnchorType(AnchorType _type);				//アンカーの種類変更

		DirectX::XMFLOAT2* GetUV() { return uv; }
		void SetUVPos(float left, float right, float top, float bottom);																		//UV設定（上下左右指定）
		void SetUVPos(DirectX::XMFLOAT2 left_top, DirectX::XMFLOAT2 right_top, DirectX::XMFLOAT2 left_bottom, DirectX::XMFLOAT2 right_bottom);	//UV設定（頂点指定）
		void SetUVPos(DirectX::XMFLOAT2 split, DirectX::XMFLOAT2 pos);																			//UV設定（分割数と順番指定）

		DirectX::XMFLOAT4 GetVertexColor() { return color; }				//頂点カラー取得
		void SetVertexColor(DirectX::XMFLOAT4 _color) { color = _color; }	//頂点カラーセット

		void SetUVState(UV_STATE _state) { uvState = _state; }		//UV座標の反転状態のセット

	protected:

		std::string texFileName;

		float width;
		float height;
		AnchorType anchorType;

		DirectX::XMFLOAT2 uv[4];
		DirectX::XMFLOAT4 color;

		static Quad2D quad;
		static unsigned int counter;

		UV_STATE uvState;
		bool uvswitchCall;

		void UpdatePosition(DirectX::XMFLOAT4X4& _mtx, Vector3& _pos);
	};


}

