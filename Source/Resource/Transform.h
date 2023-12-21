#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#include "../../../External/imgui/imgui.h"
#include "../../../External/imgui/imgui_internal.h"

class Transform
{
public:
	enum class CoordinateSystem
	{
		cRightYup,
		cLeftYup,
		cRightZup,
		cLeftZup,
		cNone,
	};

	Transform() :
		position(DirectX::XMFLOAT3())
		, scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f))
		, rotation(DirectX::XMFLOAT4())
		, scaleFactor(1.0f)
		, coordinateSystem(static_cast<int>(CoordinateSystem::cRightYup))
	{}
    ~Transform() {}

	// ---------- ImGui�p�֐� ----------
	void DrawDebug();
	void Reset();
	// ---------- ImGui�p�֐� ----------

public:// Setter,Getter, etc...
    [[nodiscard]] DirectX::XMMATRIX CalcWorld();								// ���[���h�s��Z�o
	[[nodiscard]] DirectX::XMMATRIX CalcWorldMatrix(const float& scaleFactor);	// ���W�n���|�������[���h�s��Z�o
	[[nodiscard]] DirectX::XMFLOAT3 CalcForward()	const;						// �O�����x�N�g���Z�o
	[[nodiscard]] DirectX::XMFLOAT3 CalcUp()		const;						// ������x�N�g���Z�o
	[[nodiscard]] DirectX::XMFLOAT3 CalcRight()		const;						// �E�����x�N�g���Z�o

	void SetWorld(DirectX::XMMATRIX world);

	//---------- �ʒu ( Position ) ----------
	[[nodiscard]] const DirectX::XMFLOAT3& GetPosition() const { return position; }
	[[nodiscard]] const float GetPositionX() const { return position.x; }
	[[nodiscard]] const float GetPositionY() const { return position.y; }
	[[nodiscard]] const float GetPositionZ() const { return position.z; }

	void SetPosition(const DirectX::XMFLOAT3& pos) { position = pos; }
	void SetPosition(const float& x, const float& y, const float& z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
	void SetPositionX(const float& posX) { position.x = posX; }
	void SetPositionY(const float& posY) { position.y = posY; }
	void SetPositionZ(const float& posZ) { position.z = posZ; }

	void AddPosition(const DirectX::XMFLOAT3& pos)
	{
		position.x += pos.x;
		position.y += pos.y;
		position.z += pos.z;
	}
	void AddPositionX(const float posX) { position.x += posX; }
	void AddPositionY(const float posY) { position.y += posY; }
	void AddPositionZ(const float posZ) { position.z += posZ; }

	//---------- �傫�� ( Scale ) ----------
	[[nodiscard]] DirectX::XMFLOAT3 GetScale() const { return scale; }

	void SetScale(const DirectX::XMFLOAT3& scl) { scale = scl; }

	// ScaleFactor
	[[nodiscard]] float GetScaleFactor() const { return scaleFactor; }
	void SetScaleFactor(float f) { scaleFactor = f; }

	//---------- ��] ( Rotation ) ----------
	[[nodiscard]] DirectX::XMFLOAT4 GetRotation() const { return rotation; }
	[[nodiscard]] const float GetRotationX() const { return rotation.x; }
	[[nodiscard]] const float GetRotationY() const { return rotation.y; }
	[[nodiscard]] const float GetRotationZ() const { return rotation.z; }

	void SetRotation(const DirectX::XMFLOAT4& rot) { rotation = rot; }
	void SetRotationX(const float rotX) { rotation.x = rotX; }
	void SetRotationY(const float rotY) { rotation.y = rotY; }
	void SetRotationZ(const float rotZ) { rotation.z = rotZ; }

	void AddRotationX(const float rotX) { rotation.x += rotX; }
	void AddRotationY(const float rotY) { rotation.y += rotY; }
	void AddRotationZ(const float rotZ) { rotation.z += rotZ; }

private:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 scale;
    DirectX::XMFLOAT4 rotation;

	float scaleFactor;

	int coordinateSystem;
	const DirectX::XMFLOAT4X4 CoordinateSystemTransforms[static_cast<int>(CoordinateSystem::cNone)]
	{
		{
			-1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1,
		},

		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		},

		{
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		},
		{
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		}
	};

	const std::string coordinateSystemName[static_cast<int>(CoordinateSystem::cNone)]
	{
		"Right Hand, Y up",
		"Left Hand, Y up",
		"Right Hand, Z up",
		"Left Hand, Z up"
	};
};

