#pragma once
#include "../Buffer/StructuredBuffer.h"
#include "../Buffer/ConstBuffer.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../DX11System/DX11Settransform.h"
#include "../../DX11System/ImageResourceManager.h"
#include "../../Component/ComputeComponent.h"
#include "../../ComputeDateHeader.h"
#include "../../DX11System/CLight.h"
#include "../ObjectShape/Sphere.h"
#include "../Map/TileLine.h"
#include "SphereModel.h"
#include "../../Component/ModelRenderInstance.h"
#include "SomeSupportCompute.h"
#include "../PostEffect/RenderTarget2D.h"
#include "ComputeDetail.h"
#include "../PostEffect/BloomPostEffect.h"

const UINT NUM_ELEMENTS = 1 << 13;

class ComputeManager
{
	//---pos---//
	DirectX::XMFLOAT4X4 worldmtx;

	// �f�o�b�N�p�ϐ�
	bool m_debug = false;						// �f�o�b�N�̉�ʂ��邩
	bool initEnd = false;

	bool *m_stopFlg = new bool(false);			// �Đ���~
	bool m_bloomFlg = true;
	float m_timeSpeed = 1.0f;					// �o�ߎ��Ԃ̃X�s�[�h

	// �摜�p�ϐ�
	std::string m_filename;
	std::string m_noiseTex;

	// �}�X
#if defined( TOOLMODE)
	GameObject *tile;
	bool *tileDrawFlg = new bool(true);					// �^�C���̕`��
	bool *particleNumDrawFlg = new bool(false);			// ���̕`��
	bool *particleArrowDrawFlg = new bool(true);		// ���̕`��
	bool *particleRangeDrawFlg = new bool(true);		// ���̕`��
#endif
	// ������
	unsigned int m_MaxnumElement = 10000;		// �ő吔
	float m_startTime;							// �Đ����Ԃ̕ۑ�
	PlusData* Plus = new PlusData[PlusBufNum];	// �G�~�b�^�[�̏��
	DrawStatus m_drawStatus;					// �`����
	Component::ModelRenderInstance* model;		// �p�[�e�B�N���̋��̂R�����f��
	Component::ComputeComponent* stackCompute;	// ImGui�̊J���Ă���R���s���[�g

	//---- shader ---//
	ComPtr<ID3D11VertexShader> m_pParticleVS = NULL;
	ComPtr<ID3D11GeometryShader> m_pParticleGS = NULL;
	ComPtr<ID3D11PixelShader >m_pParticlePS = NULL;

	ComPtr <ID3D11ComputeShader> m_pSortCS = NULL;		// �\�[�g�p�V�F�[�_�[
	ComPtr <ID3D11ComputeShader> m_pInitCS = NULL;		// �������p�V�F�[�_�[
	ComPtr <ID3D11ComputeShader> m_pParticleCS = NULL;	// ��{�X�V
	ComPtr <ID3D11ComputeShader> m_pSpawnCS = NULL;		// �����p�V�F�[�_�[
	ComPtr <ID3D11ComputeShader> m_pDeadCS = NULL;		// ���S����

	ID3D11ComputeShader* GetCSInitShader() { return m_pInitCS.Get(); }
	ID3D11ComputeShader* GetCSShader() { return m_pParticleCS.Get(); }
	ID3D11ComputeShader* GetCSSpawnShader() { return m_pSpawnCS.Get(); }
	ID3D11ComputeShader* GetCSDeadShader() { return m_pDeadCS.Get(); }

	// Blend&Sampler
	ID3D11BlendState* m_blendState;
	ID3D11SamplerState* m_sampler;

	// �p�[�e�B�N���{��
	ParticleBase* m_emitter;					// �G�~�b�^�[�̔z��
	ParticleBase* m_copyEmitter;				// �R�s�[��
	ParticleDraw* m_particleDraw;

	//--buffer
	StructuredBuffer* m_particleBuffer;			// ��{�̃p�[�e�B�N��
	StructuredBuffer* m_resultBuffer;			// �v�Z���ʎ擾��
	StructuredBuffer* m_resultTestBuffer;		// �v�Z���ʎ擾��
	ConstBuffer* m_plusBuffer;					// �eemitter�̎��\���̃o�b�t�@
	ConstBuffer* m_SortBuffer;					// �\�[�g
	SortStruct* sortStruct;						// �\�[�g

	// �\���̕ϐ�
	StructuredBuffer*	m_particleNumBuffer;	// �p�[�e�B�N�����Ǘ�
	ParticleNum*		m_particleNum;			// �������Ǘ�

	ComPtr <ID3D11ShaderResourceView> colorPictureSRV;		// �F�Ǘ��摜
	std::vector<Component::ComputeComponent*> computeList;	// �R���s���[�g�R���|�[�l���g���X�g

	void InitDate();								// �f�[�^������
	void InitShader();								// �����[���[�̏�����
	void Reset();									// �e��ݒ蕨�̃��Z�b�g
	void Copy();									// CS�v�Z���ʂ̎擾
	void BeforeDrawCS();							// �`��OCS�X�V�@�\�[�g
	void UpdateCS();								// CS�̎��s
	void BitonicSort(int gpu_data);					// �\�[�g
	void SetBufferPlusDate();						// �v���X�f�[�^�̖��ߍ���
	void ImGui();									// �f�o�b�O�pGUI
	void SaveImGui();								// �Z�[�u
	void LoadImGui();								// ���[�h
	void ResetImGui();								// �G�~�b�^�[�̃��Z�b�g
	void MenuBar();									// �o�[�̎g�p
	void DrawMenuBar();								// �o�[�̕`��
	void AddParticleImGui();						// �p�[�e�B�N���̒ǉ�
	void DrawParticleNumImGui();					// �p�[�e�B�N�����`�悷�邩
	void DrawArrowImGui();							// �p�[�e�B�N�����`�悷�邩
	bool SameNameCheck(std::string _name);			// �������O�̊m�F
	void CreateBlendState();						// �u�����h�X�e�[�g�̍쐬
	void CreateSampleState();						// �T���v���[�̍쐬

public :
	static ComputeManager* Instance() {
		static ComputeManager singleton;
		return &singleton;
	}

	// ��{�ϐ�
	void Init();
	void Update();
	void Draw();
	void AfterDraw();

	void FirstCS();												// CS�X�V���߂̏���
	void InitBuffer();											// CS������
	void CopyParticleNumBuffer();								// ����UAV������擾
	void SetComputeList(Component::ComputeComponent* _com);		// �R���s���[�g�̃I�u�W�F�N�g�̃Z�b�g
	void RemoveComputeList();									// ������0�̃I�u�W�F�N�g�̎��S
	void SetLookEmitter(Vector3 _vec);							// �_�u���N���b�N�Œ���
	std::string ListNameCheck(std::string _name);				// ���X�g���œ������O�����邩�̊m�F
	DrawStatus GetDrawStatus() { return m_drawStatus; }			// �`��̏�Ԏ擾
	bool* GetStopFlg() { return m_stopFlg;}						// �p�[�e�B�N���̍Đ���~
	void SetStopFlg(bool _flg) { *m_stopFlg = _flg; }			// �p�[�e�B�N���̍Đ���~
	void SetTimeSpeed(float _spd) { m_timeSpeed = _spd; }		// ���Ԃ̌o�߃X�s�[�h
	float &GetTimeSpeed() { return m_timeSpeed; }				// ���Ԃ̌o�߃X�s�[�h

	// �F�摜SRV�Z�b�g
	void SetColorPictureSRV(ID3D11ShaderResourceView* _srv) { colorPictureSRV = _srv; }

	ParticleNum *GetParticleNum() { return m_particleNum; }	
	ParticleBase *GetParticleBase() { return m_copyEmitter; }

	// buffer�擾
	StructuredBuffer* GetParticleNumBufferBuffer() { return m_particleNumBuffer; }		// �p�[�e�B�N�����Ǘ�
	StructuredBuffer* GetStructuredBuffer() { return m_particleBuffer; }				// �p�[�e�B�N���S�̂̃o�b�t�@
	StructuredBuffer* GetResultBuffer() { return m_resultBuffer; }						// ���ʎ擾�p�̃o�b�t�@

	// �eShader�̎擾
	ID3D11VertexShader* GetVSShader() { return m_pParticleVS.Get(); }
	ID3D11PixelShader* GetPixShader() { return m_pParticlePS.Get(); }
	ID3D11GeometryShader* GetGSShader() { return m_pParticleGS.Get(); }
	ID3D11ComputeShader* GetCSSortShader() { return m_pSortCS.Get(); }

	// buffer�̃R�s�[
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pBuffer)
	{
		ID3D11Buffer* debugbuf = NULL;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		pBuffer->GetDesc(&desc);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;
		if (SUCCEEDED(pDevice->CreateBuffer(&desc, NULL, &debugbuf)))
		{
#if defined(DEBUG) || defined(PROFILE)
			debugbuf->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Debug") - 1, "Debug");
#endif

			pd3dImmediateContext->CopyResource(debugbuf, pBuffer);
		}

		return debugbuf;
	}
};

//-----------------------------
// create by Suehara
//-----------------------------