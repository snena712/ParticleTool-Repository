#pragma once

#include <DirectXMath.h>
#include "ComponentBaseClass.h"
#include "../DX11System/Memory.h"
#include "../DX11System/DX11util.h"

namespace Component {

	class Camera :public ComponentBase {

	public:
		Camera(GameObject* Owner) : ComponentBase(Owner) {
			owner->transform->position = { 0,0,-20 };			// ���_
			DirectX::XMFLOAT3 lookat = { 0,0,0 };			// �����_
			DirectX::XMFLOAT3 up = { 0,1,0 };				// ������x�N�g��

			SetProjection(
				1.0f,
				5000.0f,
				DirectX::XM_PI / 2.0f,
				static_cast<float>(DX11GetScreenWidth()),
				static_cast<float>(DX11GetScreenHeight())
			);
		}

		Camera(GameObject* Owner, float nearclip, float farclip, float fov,
			float width, float height) :ComponentBase(Owner) {
			SetProjection(nearclip, farclip, fov, width, height);
		}

		void Init(float nearclip, float farclip, float fov,
			float width, float height) {
			SetProjection(nearclip, farclip, fov, width, height);
		}

		void SetNear(float nearclip) {
			m_near = nearclip;
		}

		void SetFar(float farclip) {
			m_far = farclip;
		}

		void SetFov(float fov) {
			m_Fov = fov;
		}

		void SetAspect(float width, float height) {
			m_Aspect = width / height;
		}

		void SetProjection(float nearclip, float farclip, float fov, float width, float height) {
			SetNear(nearclip);
			SetFar(farclip);
			SetFov(fov);
			SetAspect(width, height);
			CreateProjectionMatrix();
		}

		static void SetMainCamera(Camera* _camera) {
			m_MainCamera = _camera;
		}

		void CreateCameraMatrix() {
			Vector3 eye = owner->transform->position;
			Vector3 look = owner->transform->forward() + owner->transform->position;
			Vector3 up = owner->transform->up();

			ALIGN16 DirectX::XMVECTOR Eye = DirectX::XMVectorSet(eye.x, eye.y, eye.z, 0.0f);
			ALIGN16 DirectX::XMVECTOR At = DirectX::XMVectorSet(look.x, look.y, look.z, 0.0f);
			ALIGN16 DirectX::XMVECTOR Up = DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f);

			ALIGN16 DirectX::XMMATRIX camera;
			camera = DirectX::XMMatrixLookAtLH(Eye, At, Up);

			XMStoreFloat4x4(&m_Camera, camera);
		}

		void CreateProjectionMatrix() {

			ALIGN16 DirectX::XMMATRIX projection;

			projection = DirectX::XMMatrixPerspectiveFovLH(m_Fov, m_Aspect, m_near, m_far);

			XMStoreFloat4x4(&m_Projection, projection);

		}

		const DirectX::XMFLOAT4X4& GetCameraMatrix() {
			return m_Camera;
		}

		const DirectX::XMFLOAT4X4& GetProjectionMatrix() {
			return m_Projection;
		}

		float GetFov() const {
			return m_Fov;
		}

		static Camera* GetMainCamera() {
			return m_MainCamera;
		}

		void LateUpdate();
		void Draw();

#ifdef DEBUG
		void ImGuiDebug();
#endif

	private:

		static Camera* m_MainCamera;				// ���C���J�����ɐݒ肵���J�����R���|�[�l���g�̃|�C���^

		DirectX::XMFLOAT4X4		m_Projection;		// �v���W�F�N�V�����ϊ��s��
		DirectX::XMFLOAT4X4		m_Camera;			// �r���[�ϊ��s��

		float			m_near;				//�j�A�N���b�v
		float			m_Aspect;			//�A�X�y�N�g
		float			m_Fov;				//����p
		float			m_far;				//�t�@�[�N���b�v
	};

}