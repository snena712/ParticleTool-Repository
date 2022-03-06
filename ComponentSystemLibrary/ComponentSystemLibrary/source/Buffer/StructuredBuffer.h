#pragma once

#include	<d3d11.h>
#include "../../DX11System/CDirectxGraphics.h"

#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

/// �\�����o�b�t�@

class StructuredBuffer
{
private:

	// �o�b�t�@�[�ƃ��\�[�X�̌݊���
	ComPtr <ID3D11Buffer> m_buffer = { nullptr };
	ComPtr <ID3D11ShaderResourceView> m_SRV;
	ComPtr <ID3D11UnorderedAccessView> m_UAV;

	void* m_buffersOnCPU[2] = { nullptr };		//CPU������A�N�Z�X�ł��邷��X�g���N�`���o�b�t�@�̃A�h���X�B
	int m_numElement = 0;						//�v�f���B
	int m_sizeOfElement = 0;					//�G�������g�̃T�C�Y�B

public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~StructuredBuffer();

	// UAV�̃f�[�^���㏑���R�s�[���ăo�b�t�@���擾�������
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Buffer* pBuffer);	

	// �o�b�t�@�̃f�[�^���㏑���R�s�[ from_to
	void Copy(ID3D11Buffer* _to, ID3D11Buffer* _from);

	// SRV�擾
	ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }

	//UAV�̎擾
	ID3D11UnorderedAccessView* GetUAV() { return m_UAV.Get(); }

	// �o�b�t�@�[�擾
	ID3D11Buffer* GetBuffer() { 
		return m_buffer.Get(); }

public:

	//--------------------------------------------------------------------------------------
	// Helper for creating structured buffers with an SRV and UAV
	//--------------------------------------------------------------------------------------
	template <class T>
	HRESULT CreateStructuredBuffer(UINT iNumElements, const T* pInitialData)
	{
		HRESULT hr = S_OK;

		auto m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
		auto* m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();

		// Create SB
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = iNumElements * sizeof(T);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA bufferInitData;
		ZeroMemory(&bufferInitData, sizeof(bufferInitData));
		bufferInitData.pSysMem = pInitialData;
		m_device->CreateBuffer(&bufferDesc, (pInitialData) ? &bufferInitData : NULL, &m_buffer);

		// Create SRV
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		m_buffer->GetDesc(&descBuf);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		desc.BufferEx.FirstElement = 0;

		if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
		{
			// This is a Raw Buffer

			desc.Format = DXGI_FORMAT_R32_TYPELESS;
			desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
			desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
		}
		else
			if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				// This is a Structured Buffer

				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
				m_device->CreateShaderResourceView(m_buffer.Get(), &desc, &m_SRV);

			}
			else
			{
				return E_INVALIDARG;
			}

		// Create UAV
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = iNumElements;
		m_device->CreateUnorderedAccessView(m_buffer.Get(), &uavDesc, &m_UAV);

		// �T�C�Y�Ɨʂ̏�����
		m_sizeOfElement = sizeof(T);
		m_numElement = iNumElements;

		return hr;
	};
};