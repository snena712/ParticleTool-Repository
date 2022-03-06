#pragma once
#include "IRenderBase.h"
#include <d3d11.h>
#include "../ComputeDateHeader.h"

namespace Component {
	class ModelRenderInstance :public IRenderBase {

		enum FILETYPE {
			eDATFILE,
			eASSIMPFILE
		};
		FILETYPE	m_filetype = eASSIMPFILE;	// �t�@�C���^�C�v

	public:
		std::string	m_assimpfileName;			// assimpfile��
		std::string m_VertexShaderName;			// ���_�V�F�[�_�[�t�@�C����
		std::string m_PixelShaderName;			// �s�N�Z���V�F�[�_�[�t�@�C����

		ModelRenderInstance(GameObject* _owner, const char* filename, const char* vsfile, const char* psfile, const char* texfolder);
		~ModelRenderInstance();

		void DrawSetting();
		void Draw(int _num);

#ifdef DEBUG
		void ImGuiDebug();
#endif

	};
}

//-----------------------------
// create by Suehara
//-----------------------------