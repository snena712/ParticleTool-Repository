#pragma once
#include "ComponentBaseClass.h"
#include "../source/Buffer/ConstBuffer.h"
#include "../source/Buffer/StructuredBuffer.h"
#include "../DX11System\Shader.h"
#include "../DX11System\CDirectxGraphics.h"
#include "../Component/QuadRenderComponent.h"
#include "../DX11System/ImageResourceManager.h"
#include "../ComputeDateHeader.h"
#include "../source/Compute/ComputeBitChanger.h"
#include "CameraComponent.h"
#include "../DX11System/RandomClass.h"
#include <iostream>
#include <fstream> 
#include <sstream> 
#include <windows.h>
#include <tchar.h>
#include "../DX11System/CDirectInput.h"
#include "../DX11System/Time.h"
#include "../source/Compute/WindowFileDealer.h"
#include "../source/UI/Arrow/ArrowUI.h"
#include "../source/Frame/BoxFrame.h"

namespace Component {
	class ComputeComponent :public IRenderBase {

	private:
		//-----�e��V�F�[�_�[----//
		std::string m_emitterName;		// �e�̖��O
		std::string m_colorPictureName;	// �e�̖��O

		// component
#if defined( TOOLMODE)
		Component::ArrowUI* arrow;
		GameObject* m_boxFrame;
#endif

		//-- �f�o�b�O --//
		bool m_debugOpen = false;							// ���C����Imgui�J�t���O
		bool waveFlg = false;								// �g�m�C�Y�t���O
		bool colorMoveFlg = false;							// �F�ɂ��ړ��t���O
		char buf[32];										// �摜����
		int m_stackSpawnNum = 0;							// ��~���ɐ����̐����O�ɂ��Ĉꎞ�I�ɂ����֕ۑ�
		Vector3 m_nowRot;									// �G�~�b�^�[�̐i�s�����p�x
		Shape veloShape;									// �͂̌`		
		ColorControl colorControl;							// �F�̊Ǘ��ϐ�
		int item_current = 0;								// �ړ��Ǘ��p�ϐ�	
		int m_simulationCnt = 0;								// �ړ��Ǘ��p�ϐ�	
		int m_colorPictureCnt = 0;							// �F�摜
		ComPtr <ID3D11ShaderResourceView> colorPictureSRV;	// �摜SRV		

		//-- �t�@�C�� --//
		std::string filename;								// �Z�[�u���[�h�p�̊֐�	
		std::string fileFormat = "efe";						// �t�@�C���`��

		//-- �G�~�b�^�[��� --//	
		StructuredBuffer* m_PlusStructBuffer;		// �G�~�b�^�[�̒l�̃o�b�t�@�[
		PlusData* plusData = new PlusData;			// �G�~�b�^�[�̒l�@

		//-- �o�ߎ��ԂƐ����\���� --//
		float m_elapsedTime = 0;		// �����J�n����o�ߎ���
		float m_liveTime = 0;			// �p�[�e�B�N������������鎞��

		//-- �Đ����� --//
		bool m_startFlg = true;			// �Đ��t���O
		bool m_loopFlg = false;			// ���[�v�t���O
		bool m_lookFlg = false;			// �������邩�H

		// ��{�֐�
		void Init();					// ������
		void ComputeUpdate();			// �R���s���[�g�̍X�V
		void NoLoopTimerUpdate();		// ���[�v�t���O�̂Ȃ����̂̎��Ԍo�߂Ə���
		void UpdateVelocity();			// �ړ��ʂ̍X�V
		void BitFlgUpdate();			// �r�b�g�t���O�̍X�V
		void LoadColorPicture();		// �摜�擾

		// �Z�[�u���[�h�p�̕���
		const std::string nameStr = "name";
		const std::string spawnNumStr = "spawnNum";
		const std::string lifeStr = "life";
		const std::string velStr = "vel";
		const std::string scaleAnim_initScaleStr = "scaleAnim_initScale";
		const std::string scaleAnim_finishScaleStr = "scaleAnim_finishScale";
		const std::string spawnPosStr = "spawnPos";
		const std::string speedStr = "speed";
		const std::string velRangeStr = "velRange";
		const std::string forwardStr = "rot";
		const std::string gravityStr = "gravity";
		const std::string velFlgStr = "velFlgType";
		const std::string InitColorStr = "InitColor";
		const std::string EndColorStr = "EndColor";
		const std::string spawnPosRangeStr = "spawnPosRange";
		const std::string liveTimeSrt = "liveTimeSrt";
		const std::string loopFlgSrt = "loopFlgSrt";

	public:
		ComputeComponent(GameObject* Owner, const char* _filename, const char* _efeName);
		~ComputeComponent();

		void Start();
		void Update();
		void DrawObject() {};

		void SetSpawnPos(Vector3 _pos) { plusData->spawnPos = _pos; }
		PlusData GetPlusData() { return *plusData; }

		// �f�o�b�O�p
		void DebugImGui();
		bool DebugImGuiOpen();

		// �Z�[�u���[�h�p�̊֐�
		void Save();
		void WindowsLoad();
		void NormalLoad(std::string _str);
		void LoadDeal(std::string _str);

		// ImGui�̊J�t���O
		bool GetOpenFlg() { return m_debugOpen; }
		void ResetOpenFlg() { m_debugOpen = false; }

		// �t���O�擾
		bool GetLoopFlg() { return m_loopFlg; }							// �p�[�e�B�N���̃��[�v
		bool GetStartFlg() { return m_startFlg; }						// �p�[�e�B�N���̍Đ���~
		void SetStartFlg(bool _flg) { m_startFlg = _flg; }				// �p�[�e�B�N���̍Đ���~
		void ResetElapsedTime() { m_elapsedTime = 0; }
		void SetInitColor(XMFLOAT4 _col) { plusData->color.initColor = _col; }
		void SetEndColor(XMFLOAT4 _col) { plusData->color.initColor = _col; }

		// �G�~�b�^�[���̎擾
		std::string GetEmitterName() { return m_emitterName; }
		void SetEmitterName(std::string _name) { m_emitterName = _name; }

		// �摜�Ǘ�����
		void DrawPictureColor();								// �f�o�b�O�`��
		XMFLOAT2 GetPictureSize(std::string _str);				// �摜�̃T�C�Y�擾
		ID3D11ShaderResourceView* GetColorPictureSRV() {		// SRV�擾
			return colorPictureSRV.Get();
		}

#if defined( TOOLMODE)
		// ���̕`��
		void ArrowDraw() { arrow->DrawLine(plusData->spawnPos); }
		void BoxFrameDraw() {
			m_boxFrame->transform->position = Vector3(plusData->spawnPos.x, plusData->spawnPos.y, plusData->spawnPos.z);
			m_boxFrame->transform->scale = Vector3(plusData->spawnPosRange.x, plusData->spawnPosRange.y, plusData->spawnPosRange.z);
			m_boxFrame->DrawObject();
			m_boxFrame->SetExistState(false);
		}
#endif

	};
}

// colorPictureSRV ����������SRV�擾����悤�ɂ���