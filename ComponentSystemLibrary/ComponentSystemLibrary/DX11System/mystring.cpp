#include	<vector>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	"shaderhashmap.h"
#include	"mystring.h"
#include	"shader.h"
#include	"DX11Settransform.h"
#include	"dx11mathutil.h"
#include	"DX11util.h"

namespace MyString {

	using namespace DirectX;
	using  Microsoft::WRL::ComPtr;

	// �g�p���钸�_�V�F�[�_�[��
	static const char* vsfilename[] = {
		"shader/basic2Dvs.hlsl"
	};

	// �g�p����s�N�Z���V�F�[�_�[��
	static const char* psfilename[] = {
		"shader/basic2Dps.hlsl",
	};

	// ���_�\����
	struct Vertex {
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
		XMFLOAT2 Tex;
	};

	// �C���f�b�N�X�\����
	struct Face {
		unsigned int idx[3];
	};

	// ���_�f�[�^
	static std::vector<Vertex> g_vertices;

	// �C���f�b�N�X�f�[�^
	static std::vector<Face> g_indices;

	// �����R�[�h�ƕ\�����邽�߂̂t�u���W
	struct mychar {
		char	code;			// �����R�[�h
		int x;					// �\���ʒu�w
		int y;					// �\���ʒu�x
		int	width;				// ��
		int height;				// ����
		XMFLOAT4 color;			// �F
		XMFLOAT2 uv[4];			// UV���W
	};

	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11Buffer>	g_VertexBuffer;
	ComPtr<ID3D11Buffer>	g_IndexBuffer;

	// �`�r�b�h�h�t�H���g���\�[�X�A�r�q�u
	ComPtr<ID3D11Resource>	g_TexResource;
	ComPtr<ID3D11ShaderResourceView>	g_SRV;

	// �\��������(�ő�P�O�Q�S����)
	std::vector<mychar> g_mystring(MAXMOJICNT);

	// ���_�o�b�t�@�X�V
	static void UpdateVertexBuffer() {
		D3D11_MAPPED_SUBRESOURCE pData;
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();

		HRESULT hr = devcontext->Map(g_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(g_vertices.data()), sizeof(Vertex) * g_vertices.size());
			devcontext->Unmap(g_VertexBuffer.Get(), 0);
		}
	}

	// �C���f�b�N�X�o�b�t�@�X�V
	static void UpdateIndexBuffer() {
		D3D11_MAPPED_SUBRESOURCE pData;
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();

		HRESULT hr = devcontext->Map(g_IndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(g_indices.data()), sizeof(Face) * g_indices.size());
			devcontext->Unmap(g_IndexBuffer.Get(), 0);
		}
	}

	// �߂�l�@FACE��
	int CreateVertexIndexDataFirst(int length) {
		int quadnum = 0;			// �l�p�`�̐�

		// ���_������
		for (int idx = 0; idx < length; idx++) {
			Vertex v;
			v.Color = XMFLOAT4(0, 0, 0, 0);
			v.Pos.x = 0.0f;
			v.Pos.y = 0.0f;
			v.Pos.z = 0.0f;
			v.Tex.x = 0.0f;
			v.Tex.y = 0.0f;

			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);

			Face f;

			f.idx[0] = quadnum * 4;
			f.idx[1] = quadnum * 4 + 1;
			f.idx[2] = quadnum * 4 + 2;

			g_indices.emplace_back(f);

			f.idx[0] = quadnum * 4 + 1;
			f.idx[1] = quadnum * 4 + 3;
			f.idx[2] = quadnum * 4 + 2;

			g_indices.emplace_back(f);
		}

		return g_indices.size();
	}

	// �߂�l�@FACE��
	int CreateVertexIndexData(int length) {
		int quadnum = 0;			// �l�p�`�̐�

		// ���_������
		for (int idx = 0; idx < length; idx++) {
			Vertex v;

			v.Color = g_mystring[idx].color;
			v.Pos.x = static_cast<float>(g_mystring[idx].x);
			v.Pos.y = static_cast<float>(g_mystring[idx].y);
			v.Pos.z = 0.0f;
			v.Tex.x = g_mystring[idx].uv[0].x;
			v.Tex.y = g_mystring[idx].uv[0].y;
			g_vertices.push_back(v);

			v.Color = g_mystring[idx].color;
			v.Pos.x = static_cast<float>(g_mystring[idx].x + g_mystring[idx].width);
			v.Pos.y = static_cast<float>(g_mystring[idx].y);
			v.Pos.z = 0.0f;
			v.Tex.x = g_mystring[idx].uv[1].x;
			v.Tex.y = g_mystring[idx].uv[1].y;
			g_vertices.push_back(v);

			v.Color = g_mystring[idx].color;
			v.Pos.x = static_cast<float>(g_mystring[idx].x);
			v.Pos.y = static_cast<float>(g_mystring[idx].y + g_mystring[idx].height);
			v.Pos.z = 0.0f;
			v.Tex.x = g_mystring[idx].uv[2].x;
			v.Tex.y = g_mystring[idx].uv[2].y;
			g_vertices.push_back(v);

			v.Color = g_mystring[idx].color;
			v.Pos.x = static_cast<float>(g_mystring[idx].x + g_mystring[idx].width);
			v.Pos.y = static_cast<float>(g_mystring[idx].y + g_mystring[idx].height);
			v.Pos.z = 0.0f;
			v.Tex.x = g_mystring[idx].uv[3].x;
			v.Tex.y = g_mystring[idx].uv[3].y;
			g_vertices.push_back(v);

			Face f;

			f.idx[0] = quadnum * 4;
			f.idx[1] = quadnum * 4 + 1;
			f.idx[2] = quadnum * 4 + 2;

			g_indices.push_back(f);

			f.idx[0] = quadnum * 4 + 1;
			f.idx[1] = quadnum * 4 + 3;
			f.idx[2] = quadnum * 4 + 2;

			g_indices.push_back(f);

			quadnum++;
		}

		return g_indices.size();
	}

	// ������
	bool InitMyString() {

		int cnt = 0;

		// �f�o�C�X���擾
		ID3D11Device* device;
		device = GetDX11Device();

		// �f�o�C�X�R���e�L�X�g���擾
		ID3D11DeviceContext* devicecontext;
		devicecontext = GetDX11DeviceContext();

		// ������T�C�Y������
		g_mystring.clear();

		// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̏������f�[�^����
		CreateVertexIndexDataFirst(MAXMOJICNT);

		// ���_�o�b�t�@����
		bool sts = CreateVertexBufferWrite(device, sizeof(Vertex), MAXMOJICNT * 4, g_vertices.data(), &g_VertexBuffer);
		if (!sts) {
			MessageBox(nullptr, "CreateVerteBuffer error", "error", MB_OK);
			return false;
		}

		// �C���f�b�N�X�o�b�t�@����
		sts = CreateIndexBufferWrite(device, MAXMOJICNT * 4 * 2, g_indices.data(), &g_IndexBuffer);
		if (!sts) {
			MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�𐶐�
		sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
		if (!sts) {
			MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
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

		// SRV����
		sts = CreateSRVfromFile("assets/sysfont.tga", device, devicecontext, &g_TexResource, &g_SRV);
		if (!sts) {
			MessageBox(nullptr, "CreateSRVfromFile error(sysfont.tga)", "error", MB_OK);
			return false;
		}

		// VECTOR������
		ClearMyString();

		return true;
	}

	// ������`��
	void DrawMyString() {

		XMFLOAT4X4 mtx;

		CreateVertexIndexData(static_cast<int>(g_mystring.size()));

		// ���_�o�b�t�@�X�V
		UpdateVertexBuffer();

		// �C���f�b�N�X�o�b�t�@�X�V
		UpdateIndexBuffer();

		// �f�o�C�X�R���e�L�X�g���擾
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();

		// ���W�ϊ��p�̍s����Z�b�g
		DX11MtxIdentity(mtx);
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

		unsigned int stride = sizeof(Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
		unsigned  offset = 0;					// �I�t�Z�b�g�l���Z�b�g

		// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
		devcontext->IASetVertexBuffers(
			0,									// �X�^�[�g�X���b�g
			1,									// ���_�o�b�t�@��
			g_VertexBuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
			&stride,							// �X�g���C�h
			&offset);							// �I�t�Z�b�g

												// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
		devcontext->IASetIndexBuffer(
			g_IndexBuffer.Get(),				// �C���f�b�N�X�o�b�t�@
			DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
			0);									// �I�t�Z�b�g

												// �g�|���W�[���Z�b�g
		devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
		ID3D11VertexShader* vs;
		vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
		ID3D11PixelShader* ps;
		ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

		// ���_���C�A�E�g�擾
		ID3D11InputLayout* layout;

		layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

		// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
		devcontext->VSSetShader(vs, nullptr, 0);
		devcontext->PSSetShader(ps, nullptr, 0);

		// ���_�t�H�[�}�b�g���Z�b�g
		devcontext->IASetInputLayout(layout);

		// SRV���Z�b�g
		devcontext->PSSetShaderResources(0, 1, g_SRV.GetAddressOf());

		// �h���[�R�[�����s
		devcontext->DrawIndexed(
			g_indices.size()*3,		// �C���f�b�N�X��
			0,		// �J�n�C���f�b�N�X
			0);		// ����_�C���f�b�N�X

		ClearMyString();
	}

	// �`�悷�镶������N���A
	void ClearMyString() {
		// �`�敶������N���A
		g_mystring.clear();

		// �C���f�b�N�X�N���A
		g_indices.clear();

		// ���_�N���A
		g_vertices.clear();
	}

	// �A�X�L�[�t�H���g�`��p�̂t�u���W�擾
	void GetAsciiFontUV(char code, XMFLOAT2* uv) {

		int codex;
		int codey;

		codex = code & 0x0f;
		codey = code >> 4;

		codey = codey - 2;

		float tu, tv;

		tu = codex * 0.0625f;
		tv = codey * 0.0625f;

		uv[0].x = tu;
		uv[0].y = tv;

		uv[1].x = tu + 0.0625f;
		uv[1].y = tv;

		uv[2].x = tu;
		uv[2].y = tv + 0.0625f;

		uv[3].x = tu + 0.0625f;
		uv[3].y = tv + 0.0625f;
	}

	//�@������`��
	void DrawString(int x, int y, int width, int height, XMFLOAT4 color, const char* str) {

		mychar	m;

		for (int midx = 0; midx < strlen(str); midx++) {
			m.code = str[midx];
			m.width = width;
			m.height = height;
			m.x = x + width * midx;
			m.y = y;
			m.color = color;

			// �����R�[�h����`�r�b�h�h�t�H���g�̂t�u���W�����߂�
			GetAsciiFontUV(m.code, m.uv);

			// �\�����������~����
			g_mystring.emplace_back(m);
		}
	}
}
