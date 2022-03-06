#pragma once

#include <DirectXMath.h>
#include "ComponentBaseClass.h"
#include "../DX11System/Memory.h"
#include "../DX11System/DX11util.h"

namespace Component {

	class Camera :public ComponentBase {

	public:
		Camera(GameObject* Owner) : ComponentBase(Owner) {
			owner->transform->position = { 0,0,-20 };			// 視点
			DirectX::XMFLOAT3 lookat = { 0,0,0 };			// 注視点
			DirectX::XMFLOAT3 up = { 0,1,0 };				// 上向きベクトル

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

		static Camera* m_MainCamera;				// メインカメラに設定したカメラコンポーネントのポインタ

		DirectX::XMFLOAT4X4		m_Projection;		// プロジェクション変換行列
		DirectX::XMFLOAT4X4		m_Camera;			// ビュー変換行列

		float			m_near;				//ニアクリップ
		float			m_Aspect;			//アスペクト
		float			m_Fov;				//視野角
		float			m_far;				//ファークリップ
	};

}