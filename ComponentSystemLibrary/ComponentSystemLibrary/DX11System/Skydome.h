#pragma once
#include "../Component/ModelComponent.h"

class Skydome
{
public:
	Skydome(const Skydome&) = delete;
	Skydome& operator=(const Skydome&) = delete;
	Skydome(Skydome&&) = delete;
	Skydome& operator=(Skydome&&) = delete;

	static Skydome* GetInstance() {
		static Skydome instance;
		return &instance;
	}

	void Init(const char* _filename, const char* _vsfile, const char* _psfile, const char* _texfolder);
	void Exit();
	void Draw();

	bool GetDrawState() { return m_drawExists; }
	void SetDrawState(bool _newState) { m_drawExists = _newState; }

	float GetScale() { return m_scale; }
	void SetScale(float _scale) {
		if (_scale < 1.0f) return;
		m_scale = _scale;
	}

private:
	Skydome() {};
	~Skydome() {};

	std::string	m_assimpfileName;			// assimpfile名
	std::string m_VertexShaderName;			// 頂点シェーダーファイル名
	std::string m_PixelShaderName;			// ピクセルシェーダーファイル名

	bool m_drawExists;	//描画するか否か
	float m_scale;

};

