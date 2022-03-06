#include	"Cone.h"

#define VER 19

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void Cone::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// �C���f�b�N�X�f�[�^���쐬
void Cone::CreateIndex() {

	// �C���f�b�N�X����
	for (int i = 0; i < (VER - 1); i++)
	{
		Face f;
		f.idx[0] = i * 2 + 0;
		f.idx[1] = i * 2 + 1;
		f.idx[2] = i * 2 + 2;

		m_face.emplace_back(f);
	}
}

// ���_�f�[�^���쐬
void Cone::CreateVertex() {

	Vertex ve;

	for (DWORD i = 0; i < VER; i++)
	{
		float v = (float)i / (float)(VER - 1);
		float theta = v * 3.14f * 2;
		ve.Pos = XMFLOAT3(sinf(theta), -0.7f, cosf(theta));
		ve.Normal = XMFLOAT3(sinf(theta), 0.7f, cosf(theta));
		ve.Color = m_color;
		m_vertex.emplace_back(ve);

		ve.Pos = XMFLOAT3(0.0f, 0.7f, 0.0f);
		ve.Normal = XMFLOAT3(sinf(theta), 0.7f, cosf(theta));
		ve.Color = m_color;
		m_vertex.emplace_back(ve);
	}

	// Vertices �Ō�͐擪�̍��W�Ɠ���
	ve.Pos = m_vertex[0].Pos;
	m_vertex.emplace_back(ve);
	ve.Pos = m_vertex[1].Pos;
	m_vertex.emplace_back(ve);
}

// �`��
void Cone::Draw() {

	ID3D11DeviceContext* _devicecon = GetDX11DeviceContext();

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_devicecon->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	_devicecon->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
	_devicecon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	_devicecon->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));					// ���_���C�A�E�g�Z�b�g

	_devicecon->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	_devicecon->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g

	_devicecon->DrawIndexed(
		static_cast<unsigned int>(m_face.size() * 3),		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
		0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
		0);									// ���_�o�b�t�@�̍ŏ�����g��
}

bool Cone::Init(
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
		_device,							// �f�o�C�X�I�u�W�F�N�g
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
// ���t���[���X�V��ύX����
void Cone::BufferUpdate(XMFLOAT4 _color, Vector3 _pos)
{
	// �ۑ����Ă���F�ƈقȂ鎞�̂ݎ��s
	/*if (m_color.x != _color.x || m_color.y != _color.y || m_color.z != _color.z || m_color.z != _color.z) {*/

	HRESULT hr = S_OK;

	void* _date = m_vertex.data();

	for (int i = 0; i < VER * 2; ++i)
	{
		m_vertex[i].Color = _color;
		if (i % 2 == 1)
		{
			m_vertex[i].Pos.y = _pos.y;
		}
	}

	// buffer�̍X�V
	D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
	GetDX11DeviceContext()->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	memcpy(MappedResource.pData, m_vertex.data(), sizeof(Vertex) * m_vertex.size()); // 3���_���R�s�[
	GetDX11DeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);

}


void Cone::SetTransform(Transform* _tra, Vector3 _pos, XMFLOAT4 _qt, float _Radius, float _size)
{
	DirectX::XMFLOAT4X4 make, mtx, scale;

	DX11MtxScale(_Radius * _size, _Radius * _size, _Radius * _size, scale);

	// ��]
	DirectX::XMFLOAT4 q, axis;
	axis = _tra->right();
	DX11QtRotationAxis(q, axis, -90);
	DX11QtMul(_qt, _qt, q);

	// qt����s��쐬
	DX11MtxFromQt(mtx, _qt);
	DX11MtxMultiply(make, scale, mtx);

	make._41 = _pos.x;
	make._42 = _pos.y;
	make._43 = _pos.z;

	// �o�b�t�@�ɐݒ�
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);
}