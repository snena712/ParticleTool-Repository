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

	std::string	m_assimpfileName;			// assimpfile��
	std::string m_VertexShaderName;			// ���_�V�F�[�_�[�t�@�C����
	std::string m_PixelShaderName;			// �s�N�Z���V�F�[�_�[�t�@�C����

	bool m_drawExists;	//�`�悷�邩�ۂ�
	float m_scale;

};

