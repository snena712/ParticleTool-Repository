#include "StructuredBuffer.h"
StructuredBuffer::~StructuredBuffer()
{
}




//--------------------------------------------------------------------------------------
// Create a CPU accessible buffer and download the content of a GPU buffer into it
// This function is very useful for debugging CS programs
//-------------------------------------------------------------------------------------- 
ID3D11Buffer* StructuredBuffer::CreateAndCopyToDebugBuf(ID3D11Buffer* pBuffer)
{
	ID3D11Buffer* debugbuf = NULL;
	auto m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
	auto* m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	pBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	if (SUCCEEDED(m_device->CreateBuffer(&desc, NULL, &debugbuf)))
	{
#if defined(DEBUG) || defined(PROFILE)
		debugbuf->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Debug") - 1, "Debug");
#endif

		m_deviceContex->CopyResource(debugbuf, pBuffer);
	}

	return debugbuf;
}


void StructuredBuffer::Copy(ID3D11Buffer* _to, ID3D11Buffer* _from)
{
	auto* m_deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
	m_deviceContex->CopyResource(_to, _from);
}





