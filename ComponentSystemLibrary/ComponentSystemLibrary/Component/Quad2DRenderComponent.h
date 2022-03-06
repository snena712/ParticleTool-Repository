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

		std::string GetTexFileName() { return texFileName; }				//�e�N�X�`�����̎擾
		std::string* GetTexFileNameAdress() { return &texFileName; }		//�A�h���X�n���i��{�g��Ȃ��j

		void ChangeTexture(const char* _fileName);							//���I�ȃe�N�X�`���ύX

		float GetWidth() { return width; }
		void SetWidth(float _width) { width = _width; }
		float GetHeight() { return height; }
		void SetHeight(float _height) { height = _height; }

		AnchorType GetAnchorType() { return anchorType; }
		void SetAnchorType(AnchorType _type);				//�A���J�[�̎�ޕύX

		DirectX::XMFLOAT2* GetUV() { return uv; }
		void SetUVPos(float left, float right, float top, float bottom);																		//UV�ݒ�i�㉺���E�w��j
		void SetUVPos(DirectX::XMFLOAT2 left_top, DirectX::XMFLOAT2 right_top, DirectX::XMFLOAT2 left_bottom, DirectX::XMFLOAT2 right_bottom);	//UV�ݒ�i���_�w��j
		void SetUVPos(DirectX::XMFLOAT2 split, DirectX::XMFLOAT2 pos);																			//UV�ݒ�i�������Ə��Ԏw��j

		DirectX::XMFLOAT4 GetVertexColor() { return color; }				//���_�J���[�擾
		void SetVertexColor(DirectX::XMFLOAT4 _color) { color = _color; }	//���_�J���[�Z�b�g

		void SetUVState(UV_STATE _state) { uvState = _state; }		//UV���W�̔��]��Ԃ̃Z�b�g

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

