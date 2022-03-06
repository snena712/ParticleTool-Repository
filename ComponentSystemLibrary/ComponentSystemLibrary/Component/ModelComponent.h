#pragma once

#include "IRenderBase.h"
#include <d3d11.h>

namespace Component {
	class ModelRender :public IRenderBase {

		enum FILETYPE {
			eDATFILE,
			eASSIMPFILE
		};
		FILETYPE	m_filetype = eASSIMPFILE;	// ファイルタイプ

	public:
		std::string	m_assimpfileName;			// assimpfile名
		std::string m_VertexShaderName;			// 頂点シェーダーファイル名
		std::string m_PixelShaderName;			// ピクセルシェーダーファイル名

		ModelRender(GameObject* _owner, const char* filename, const char* vsfile, const char* psfile, const char* texfolder);
		~ModelRender();

		void DrawObject();

#ifdef DEBUG
		void ImGuiDebug();
#endif

	};
}