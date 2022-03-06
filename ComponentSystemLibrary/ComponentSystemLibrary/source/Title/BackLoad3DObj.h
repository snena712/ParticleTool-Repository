#pragma once
#include "../Title/BackLoad3DObj.h"
#include "../../Component/GameObject.h"
#include "../../DX11System/ModelResourceManager.h"
#include "../../DX11System\dx11mathutil.h"
#include "../Title/TitlePlayer.h"

namespace Component {

	class  BackLoad3DObj :public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::TitlePlayer* player;

		void BackLoad(
			const char* _filename,
			const char* _vsfile,
			const char* _psfile,
			const char* _texfolder);

	public:
		BackLoad3DObj(GameObject* _obj);
		~BackLoad3DObj();

	};
}
