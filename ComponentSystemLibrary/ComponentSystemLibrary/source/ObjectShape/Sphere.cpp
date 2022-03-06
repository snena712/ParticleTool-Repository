#include	"Sphere.h"

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void Sphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// �C���f�b�N�X�f�[�^���쐬
void Sphere::CreateIndex() {

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1) * y + x;			// ������W�̃C���f�b�N�X

			Face f;
			// �㔼��
			f.idx[0] = count;
			f.idx[1] = count + 1;
			f.idx[2] = count + 1 + (m_divX + 1);

			m_face.emplace_back(f);

			// ������
			f.idx[0] = count;
			f.idx[1] = count + (m_divX + 1) + 1;
			f.idx[2] = count + (m_divX + 1);

			m_face.emplace_back(f);

		}
	}
}

// ���_�f�[�^���쐬
void Sphere::CreateVertex() {

	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	XMFLOAT3	Normal;
	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
	for (unsigned int y = 0; y <= m_divY; y++) {
		elevation = (PI * (float)y) / (float)m_divY;    // �p���Z�b�g
		float r = m_radius * sinf(elevation);				// �p�ɉ��������a���v�Z

		for (unsigned int x = 0; x <= m_divX; x++) {
			azimuth = (2 * PI * (float)x) / (float)m_divX;	// ���ʊp���Z�b�g

			Vertex v;
			v.Pos.x = r * cosf(azimuth);
			v.Pos.y = m_radius * cosf(elevation);
			v.Pos.z = r * sinf(azimuth);

			Normalize(v.Pos, Normal);		// �@�����v�Z
			v.Normal = Normal;				// �@�����Z�b�g

			v.Color = m_color;				// ���_�J���[

			m_vertex.emplace_back(v);
		}
	}
}

// �`��
void Sphere::Draw(unsigned int _num, ParticleBase* _particleBase) {

	ID3D11DeviceContext *_devicecon = GetDX11DeviceContext();

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_devicecon->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	_devicecon->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);						// �C���f�b�N�X�o�b�t�@���Z�b�g
	_devicecon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);							// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	_devicecon->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));	// ���_���C�A�E�g�Z�b�g

	_devicecon->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	_devicecon->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g

	//ColorUpdate(XMFLOAT4(1,1,1,1));

	for (int i = 0; i < _num; i++)
	{
		SetTransform(_particleBase[i].pos, 1, _particleBase[i].scale.x);
		// ���̒�ʐ��o�b�t�@�̍X�V

		// �`�搔���h���[�R�[���|����
		_devicecon->DrawIndexed(
			static_cast<unsigned int>(m_face.size()*3),		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
			0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
			0);									// ���_�o�b�t�@�̍ŏ�����g��
	}
}

bool Sphere::Init(float r,				// ���a
	int division_horizontal,			// ���������̕�����
	int division_vertical,				// ���������̕�����
	std::string _VS,					// ���_�V�F�[�_�[
	std::string _PS)					// �s�N�Z���V�F�[�_�[
{

	bool sts;
	// ��������ۑ�
	m_divX = division_horizontal;
	m_divY = division_vertical;

	// ���a��ۑ�
	m_radius = r;

	// �J���[�l��ۑ�
	//m_color = color;

	// �f�o�C�X�̐ݒ�
	ID3D11Device* _device = GetDX11Device();

	// ���̃C���f�b�N�X�f�[�^���쐬
	CreateIndex();

	// ���̒��_�f�[�^���쐬
	CreateVertex();

	// �V�F�[�_�[���ݒ�
	m_VertexShaderName = _VS;
	m_PixelShaderName = _PS;

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

// ���_�o�b�t�@�̐F�X�V
// ���t���[���X�V��ύX���݂̂ɕύX����
void Sphere::ColorUpdate(XMFLOAT4 _color)
{
	// �ۑ����Ă���F�ƈقȂ鎞�̂ݎ��s
	if (m_color.x != _color.x || m_color.y != _color.y || m_color.z != _color.z || m_color.z != _color.z) {

		HRESULT hr = S_OK;

		void* _date = m_vertex.data();

		// buffer�̍X�V
		D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
		hr = GetDX11DeviceContext()->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		if (SUCCEEDED(hr)) {
			Vertex* pBuffer = (Vertex*)MappedResource.pData;
			if (pBuffer[0].Color.x != 0 || pBuffer[0].Color.y != 0 || pBuffer[0].Color.z != 0 || pBuffer[0].Color.w != 0)
			{
				m_color = _color;
				for (int i = 0; i < m_divX * m_divY; ++i)
				{
					pBuffer[i].Color = _color;
				}
			}
			GetDX11DeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);
		}
	}
}

void Sphere::SetTransform(XMFLOAT3 _pos, float _Radius, float _size)
{
	DirectX::XMFLOAT4X4 make, mtx, scale;

	DX11MtxScale(_Radius * _size, _Radius * _size, _Radius * _size, scale);
	DX11MtxFromQt(mtx, XMFLOAT4(0, 0, 0, 1));
	DX11MtxMultiply(make, scale, mtx);

	make._41 = _pos.x+10;
	make._42 = _pos.y;
	make._43 = _pos.z;

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);
}