#pragma once
#include	<d3d11.h>
#include "../../DX11System/CDirectxGraphics.h"
#include <type_traits>

class ConstBuffer
{
public:
	enum class Type
	{
		Normal,	// 1つ
		Multi	// 複数
	};

private:

	ComPtr <ID3D11Buffer> m_buffer = { nullptr };
	ComPtr < ID3D11ShaderResourceView>* m_SRV;
	
	ID3D11Buffer* GetBuffer() { return m_buffer.Get(); }

	int m_size;		// エレメントの数
public:

	template <class T>
	void Update(T _date)
	{
		HRESULT hr = S_OK;
		auto m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();

		D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
		hr = m_deviceContex->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		if (SUCCEEDED(hr)) {
			memcpy_s(MappedResource.pData, MappedResource.RowPitch, _date, sizeof(*_date) * m_size);
			m_deviceContex->Unmap(m_buffer.Get(), 0);
		}
	}

	template <class T>
	void NoPointerUpdate(T _date)
	{
		auto m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
	
		m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
		m_deviceContex->UpdateSubresource(m_buffer.Get(), 0, nullptr, &_date, 0, 0);
	}

	void SetBuffer(int _num) {
		auto m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
		m_deviceContex->CSSetConstantBuffers(_num, 1, m_buffer.GetAddressOf());
	}

	void SetVSBuffer(int _num) {
		auto m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
		m_deviceContex->VSSetConstantBuffers(_num, 1, m_buffer.GetAddressOf());
	}

	void SetGSBuffer(int _num) {
		auto m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
		m_deviceContex->GSSetConstantBuffers(_num, 1, m_buffer.GetAddressOf());
	}

	void SetPSBuffer(int _num) {
		auto m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
		m_deviceContex->PSSetConstantBuffers(_num, 1, m_buffer.GetAddressOf());
	}

	//HRESULT InitSRV();

	template <class T>
	void CreateConStructuredBuffer(Type _type, UINT iNumElements)
	{
		auto m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
		auto m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();

		D3D11_BUFFER_DESC desc;
		m_size = iNumElements;
		bool sts;

		switch (_type)
		{
		case ConstBuffer::Type::Normal:
			sts = CreateConstantBuffer(
				m_device,							// デバイス
				sizeof(T),	// サイズ
				&m_buffer);							// コンスタントバッファ２
			if (!sts) {
				MessageBox(NULL, "CreateBuffer(constant buffer world) error", "Error", MB_OK);
			}
			return;

		case ConstBuffer::Type::Multi:
			ZeroMemory(&desc, sizeof(desc));
			desc.Usage = D3D11_USAGE_DYNAMIC;								// バッファ使用方法
			desc.ByteWidth = sizeof(T) * iNumElements;						// バッファの大き
			//desc.StructureByteStride = sizeof(T) * iNumElements;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					// コンスタントバッファ
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					// CPUアクセス可能
			/*desc.Usage =D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;*/
			sts = m_device->CreateBuffer(&desc, NULL, &m_buffer);

			return;
		default:
			break;
		}

		return;
	}
};
