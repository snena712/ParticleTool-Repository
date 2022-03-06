#pragma once

#include "Quad2DRenderComponent.h"
#include "../DX11System/Shader.h"
#include "../DX11System/Memory.h"

namespace Component {
	class Quad2DMultiRender :public Quad2DRender {

	public:
		Quad2DMultiRender(GameObject* _owner, const char* _firstfilename, const char* _secondfilename);	// 1つ目メイン、2つ目合成物
		~Quad2DMultiRender();

		void Start();
		void DrawUI();

		std::string GetSubTexFileName() { return texSubFileName; }			//テクスチャ名の取得
		std::string* GetSubTexFileNameAdress() { return &texSubFileName; }	//アドレス渡し（基本使わない）

		void ChangeSubTexture(const char* _fileName);						//動的なテクスチャ変更

		//==========UV=========//
		void SetVolume(float _num) { colorVolume.volume = _num; }			// 合成色の割合のセット
		float SetVolume() { return colorVolume.volume; }					// 合成色の割合の取得

		void ConstBufferInit();			// 定数バッファの初期化
	private:

		ALIGN16 struct ColorVolume
		{
			float volume;
		};

		std::string texSubFileName;		// ２つめ　<合成物>

		ColorVolume colorVolume;			// 色の割合の大きさ

		ID3D11Buffer* m_ColorVolumeBuffer = nullptr;	// コンスタントバッファ 合成色の割合
	};


}

