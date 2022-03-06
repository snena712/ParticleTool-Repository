#include	<string>
#include	"dx11mathutil.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"
#include	"../Application.h"
#include	"quad2D.h"
#include	"ImageResourceManager.h"

// comptr
using Microsoft::WRL::ComPtr;

// �g�p���钸�_�V�F�[�_�[��
const char* Quad2D::vsfilename[] = {
	"shader/quad2dvs.hlsl"	,	//�ʏ�i2D�j
};

// �g�p����s�N�Z���V�F�[�_�[��
const char* Quad2D::psfilename[] = {
	"shader/quad2dps.hlsl",		//�ʏ�i2D�j
	"shader/multips.hlsl"
};

// ��`�̏�����
bool Quad2D::Init(
	int width,
	int height,
	DirectX::XMFLOAT4 color,
	DirectX::XMFLOAT2* uv)
{
	DX11MtxIdentity(m_worldmtx);	// �����p��

	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(quad2dps.hlsl)", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(multips.hlsl)", "error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// ���_�V�F�[�_�[�t�@�C����
		layout,					// ���_���C�A�E�g
		numElements);			// �G�������g��

	// ���_������
	updateVertex(width, height, color, uv);

	// �f�o�C�X���擾
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBufferWrite(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Quad2D::Vertex),		// �P���_������o�C�g��
		4,							// ���_��
		m_vertex,					// ���_�f�[�^�i�[�������擪�A�h���X
		&m_vertexbuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�𐶐�
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		4,							// �C���f�b�N�X��
		index,						// �C���f�b�N�X�i�[�������擪�A�h���X
		&m_indexbuffer				// �C���f�b�N�X�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	// �r���[�|�[�g�T�C�Y�Z�b�g
	SetViewPortSize(Application::Instance()->CLIENT_WIDTH, Application::Instance()->CLIENT_HEIGHT);

	return true;
}

// �`��
void Quad2D::Draw() {

	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �e�N�X�`�����Z�b�g
	ID3D11ShaderResourceView* srv[] = { ImageResourceManager::GetInstance()->GetResource(m_texFileName)->GetSRV() };
	devcontext->PSSetShaderResources(0, 1, srv);

	// ���[���h�ϊ��s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad2D::Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		m_vertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�A���_���C�A�E�g�擾
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* layout = nullptr;

	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	//�V�F�[�_�[�Z�b�g
	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);

	// �h���[�R�[�����s
	devcontext->DrawIndexed(
		4,						// �C���f�b�N�X��
		0,						// �J�n�C���f�b�N�X
		0);						// ����_�C���f�b�N�X
}

// �}���`�e�N�X�`���̕`��
void Quad2D::MultiDraw()
{
	DirectX::XMFLOAT4X4 mtx;

	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ���[���h�ϊ��s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad2D::Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		m_vertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�A���_���C�A�E�g�擾
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* layout = nullptr;

	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[1]);
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);

	// �h���[�R�[�����s
	devcontext->DrawIndexed(
		4,						// �C���f�b�N�X��
		0,						// �J�n�C���f�b�N�X
		0);						// ����_�C���f�b�N�X
}

void Quad2D::Exit() {

}

bool Quad2D::SetViewPortSize(uint32_t width, uint32_t height)
{
	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	ComPtr<ID3D11Buffer> CBViewPort;

	// �r���[�|�[�g�p�̒萔�o�b�t�@
	struct ConstantBufferViewPort {
		uint32_t	Width[4];			// �r���[�|�[�g��
		uint32_t	Height[4];			// �r���[�|�[�g����
	};

	// �r���[�|�[�g�萔�o�b�t�@�ɒl���Z�b�g
	bool sts = CreateConstantBufferWrite(
		device,
		sizeof(ConstantBufferViewPort),
		&CBViewPort);
	if (!sts) {
		return false;
	}

	ConstantBufferViewPort cb;
	cb.Width[0] = width;
	cb.Height[0] = height;

	D3D11_MAPPED_SUBRESOURCE pData;

	HRESULT hr = devcontext->Map(CBViewPort.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferViewPort));
		devcontext->Unmap(CBViewPort.Get(), 0);
	}
	else {
		return false;
	}

	// ���_�V�F�[�_�[�ɃZ�b�g
	devcontext->VSSetConstantBuffers(5, 1, CBViewPort.GetAddressOf());

	return true;
}

// �g��A�k��
void Quad2D::SetScale(float sx, float sy, float sz) {
	DX11MtxScale(sx, sy, sz, m_worldmtx);
}

// �ʒu���Z�b�g
void Quad2D::SetPosition(float x, float y, float z) {
	m_worldmtx._41 = x;
	m_worldmtx._42 = y;
	m_worldmtx._43 = z;
}

// �ʒu�Ɖ�]
void Quad2D::SetPosRot(Vector3 pos, Vector3 rot) {
	Vector3 zero = Vector3::zero;
	DX11MakeWorldMatrix(m_worldmtx, zero, rot, pos);
}

void Quad2D::updateVertex(
	float width,
	float height,
	DirectX::XMFLOAT4 color,
	DirectX::XMFLOAT2* uv)
{

	m_width = width;
	m_height = height;

	Quad2D::Vertex	v[4] = {
		// ���W													// �J���[	// UV	
		XMFLOAT3(-m_width / 2.0f,	-m_height / 2.0f, 0.0f),	color,		uv[0],
		XMFLOAT3(m_width / 2.0f,	-m_height / 2.0f, 0.0f),	color,		uv[1],
		XMFLOAT3(-m_width / 2.0f,	 m_height / 2.0f, 0.0f),	color,		uv[2],
		XMFLOAT3(m_width / 2.0f,	 m_height / 2.0f, 0.0f),	color,		uv[3]
	};

	for (int i = 0; i < 4; i++) {
		m_vertex[i] = v[i];
	}
}


// ���_�o�b�t�@�X�V
void Quad2D::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(m_vertex), sizeof(Quad2D::Vertex) * 4);
		devcontext->Unmap(m_vertexbuffer.Get(), 0);
	}
}