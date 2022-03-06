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
		
		//----------�摜�T�C�Y�̐ݒ�-----------//
		float GetWidth() { return width; }								
		void SetWidth(float _width) { width = _width; }
		float GetHeight() { return height; }
		void SetHeight(float _height) { height = _height; }

		void SetPostID(int _id) { m_drawID = _id; }  // �`��ԍ��̐ݒ�
		int GetPostID() { return m_drawID; }		 // �`��ԍ���n��

		//-----------�e�N�X�`���̕ύX-----------//	
		std::string GetTexFileName() { return texFileName; }			// �e�N�X�`�����̎擾
		std::string* GetTexFileNameAdress() { return &texFileName; }	// �A�h���X�n���i��{�g��Ȃ��j

		//-----------�`��-----------//
		void Start();
		void Update();
		void DrawObject();
		void NormalDraw();		// �ʏ�̕`��ݒ�


		//---------UV�̐ݒ�---------//
		void SetUVPos(DirectX::XMFLOAT2 left_top, DirectX::XMFLOAT2 right_top, DirectX::XMFLOAT2 left_bottom, DirectX::XMFLOAT2 right_bottom);


		//-----�`�悷��G�t�F�N�g�̌��Əꏊ�ƃT�C�Y----//
		void SetPosList(std::vector<Vector3> *_pos) { poslist =*_pos; }
		void SetSizeList(std::vector<Vector3*> _size) { sizelist = _size; }

	private:
		//====================directx�̐ݒ�========================//
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_devContext;
		
		//=========================�ϐ�===========================//
		std::string texFileName;	
		std::string texSubFileName;		// �Q�߁@<������>
		std::string m_vsmultistr = "shader/vsmultibiilbord.hlsl";
		std::string m_psmultistr = "shader/multips.hlsl";
		
		float width;				// ���̑傫��
		float height;				// �c�̑傫��
		int m_drowcnt;				// �`�悷�鐔
		DirectX::XMFLOAT2 uv[4];	// uv�̈ʒu
		DirectX::XMFLOAT4 color;	// �F
		
		static PostEffectQuad postquad;					// �l�p�`
		static unsigned int  postcounter;				// �`��̌��̃J�E���g

		// �y�A�ɂ���
		std::vector<Vector3> poslist;
		std::vector<Vector3*> sizelist;

		int m_drawID;		// �`�悷�鏇��(����g���ĂȂ�)
	};
}

//-----------------------------
// create by Suehara
//-----------------------------