#include	"Box.h"

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void Box::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// �C���f�b�N�X�f�[�^���쐬
void Box::CreateIndex() {	// �C���f�b�N�X���X�g
	
	Face g_IndexList[]{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	for (auto ver : g_IndexList)
	{
		m_face.emplace_back(ver);
	}

}

// ���_�f�[�^���쐬
void Box::CreateVertex() {

	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	XMFLOAT3	Normal;
	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬

	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), m_color },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(1.0f, 1.0f, -1.0f),  m_color },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f),   m_color },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),  XMFLOAT3(-1.0f, 1.0f, 1.0f),  m_color },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT3(-1.0f, -1.0f, -1.0f),m_color },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), m_color },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(1.0f, -1.0f, 1.0f),  m_color },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), m_color },
	};

	// ���_���̓��ꍞ��
	for (auto ver : vertices)
	{
		m_vertex.emplace_back(ver);
	}

}

// �`��
void Box::Draw() {

	ID3D11DeviceContext* _devicecon = GetDX11DeviceContext();

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_devicecon->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	_devicecon->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
	_devicecon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);				// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	_devicecon->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));				// ���_���C�A�E�g�Z�b�g

	_devicecon->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	_devicecon->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g

	_devicecon->DrawIndexed(
		static_cast<unsigned int>(m_face.size() * 3),		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
		0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
		0);									// ���_�o�b�t�@�̍ŏ�����g��
}

bool Box::Init(
	XMFLOAT4 color) {

	bool sts;

	// �J���[�l��ۑ�
	m_color = color;

	// �f�o�C�X�̐ݒ�
	ID3D11Device* _device = GetDX11Device();

	// ���̃C���f�b�N�X�f�[�^���쐬
	CreateIndex();

	// ���̒��_�f�[�^���쐬
	CreateVertex();

	// ���_�o�b�t�@�쐬
	sts = CreateVertexBufferWrite(
		_device,
		sizeof(Vertex),						// �P���_������o�C�g��
		m_vertex.size(),					// ���_��
		m_vertex.data(),					// ���_�f�[�^�i�[�������擪�A�h���X
		m_pVertexBuffer.GetAddressOf());	// ���_�o�b�t�@
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	sts = CreateIndexBuffer(
		_device,								// �f�o�C�X�I�u�W�F�N�g
		m_face.size() * 3,					// �C���f�b�N�X��
		m_face.data(),						// �C���f�b�N�X�f�[�^�擪�A�h���X
		m_pIndexBuffer.GetAddressOf());		// �C���f�b�N�X�o�b�t�@
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		m_VertexShaderName,
		layout,
		numElements);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(m_PixelShaderName);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
	}

	return true;
}


// ���_�o�b�t�@�̍X�V
// ���t���[���X�V��ύX���݂̂ɕύX����
void Box::ColorUpdate(XMFLOAT4 _color)
{
	for (int i=0;i< m_vertex.size();i++)
	{
		m_vertex[i].Color = _color;
	}

	// buffer�̍X�V
	D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };

	GetDX11DeviceContext()->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	memcpy(MappedResource.pData, m_vertex.data(), sizeof(Vertex) * m_vertex.size()); // 3���_���R�s�[
	GetDX11DeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);
}

void Box::SetTransform(Transform* _tra, Vector3 _scale, Vector3 _size)
{
	// �����蔻��̘g�̕`��
	
	DirectX::XMFLOAT4X4 make, mtx, scale;

	DX11MtxScale(_scale.x * _size.x, _scale.y * _size.y, _scale.z * _size.z, scale);
	DX11MtxFromQt(mtx, _tra->qt);
	DX11MtxMultiply(make, scale, mtx);

	make._41 = _tra->position.x;
	make._42 = _tra->position.y;
	make._43 = _tra->position.z;

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);
}