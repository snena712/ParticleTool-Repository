#pragma once

#include	<d3d11.h>
#include "../../DX11System/CDirectxGraphics.h"

#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

/// 構造化バッファ

class StructuredBuffer
{
private:

	// バッファーとリソースの互換性
	ComPtr <ID3D11Buffer> m_buffer = { nullptr };
	ComPtr <ID3D11ShaderResourceView> m_SRV;
	ComPtr <ID3D11UnorderedAccessView> m_UAV;

	void* m_buffersOnCPU[2] = { nullptr };		//CPU側からアクセスできるするストラクチャバッファのアドレス。
	int m_numElement = 0;						//要素数。
	int m_sizeOfElement = 0;					//エレメントのサイズ。

public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~StructuredBuffer();

	// UAVのデータを上書きコピーしてバッファを取得するもの
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Buffer* pBuffer);	

	// バッファのデータを上書きコピー from_to
	void Copy(ID3D11Buffer* _to, ID3D11Buffer* _from);

	// SRV取得
	ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }

	//UAVの取得
	ID3D11UnorderedAccessView* GetUAV() { return m_UAV.Get(); }

	// バッファー取得
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

		// サイズと量の初期化
		m_sizeOfElement = sizeof(T);
		m_numElement = iNumElements;

		return hr;
	};
};