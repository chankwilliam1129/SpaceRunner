#include "transform.h"
#include "transform_vibrate.h"

void NormalizeRotation(float& rotation)
{
	while (rotation > 360.0f) { rotation -= 360.0f; }
	while (rotation < 0.0f) { rotation += 360.0f; }
}

void Transform::Update()
{
	NormalizeRotation(rotation.x);
	NormalizeRotation(rotation.y);
	NormalizeRotation(rotation.z);
}

void Transform::ImGuiUpdate()
{
#ifdef USE_IMGUI
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::DragFloat3("position", &position.x, 0.1f);
		ImGui::DragFloat3("rotation", &rotation.x, 0.5f);
		ImGui::DragFloat3("scale", &scale.x, 0.1f);
		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
#endif
}

DirectX::XMMATRIX Transform::GetWorldMatrix()
{
	DirectX::XMMATRIX S, R, T, world;
	world = DirectX::XMMatrixIdentity();
	Vector3 sca = scale;
	if (object->GetComponent<ScaleVibrate>() != nullptr && object->GetComponent<ScaleVibrate>()->isTurnOn)
	{
		sca.x *= object->GetComponent<ScaleVibrate>()->offset.x;
		sca.y *= object->GetComponent<ScaleVibrate>()->offset.y;
		sca.z *= object->GetComponent<ScaleVibrate>()->offset.z;
	}
	S = DirectX::XMMatrixScaling(sca.x, sca.y, sca.z);
	R = Quaternion::ToMatrix(Quaternion::Euler(rotation));

	Vector3 pos = position;
	if (object->GetComponent<PositionVibrate>() != nullptr && object->GetComponent<PositionVibrate>()->isTurnOn)
	{
		pos += object->GetComponent<PositionVibrate>()->offset;
	}

	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	world = S * R * T;

	if (object->parent != NULL)
	{
		world *= object->parent->GetComponent<Transform>()->GetWorldMatrix();
	}

	return	world;
};

Vector3 Transform::GetWorldPosition()
{
	DirectX::XMMATRIX temp;
	temp = object->GetComponent<Transform>()->GetWorldMatrix();
	if (object->parent != nullptr) { temp *= object->parent->GetComponent<Transform>()->GetWorldMatrix(); }
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, temp);
	return Vector3(world._41, world._42, world._43);
}

Vector2 Transform::GetWorldPosition2D()
{
	Vector3 pos = object->GetComponent<Transform>()->position;
	if (object->GetComponent<PositionVibrate>() != nullptr && object->GetComponent<PositionVibrate>()->isTurnOn)
	{
		pos += object->GetComponent<PositionVibrate>()->offset;
	}

	if (object->parent != nullptr)
	{
		Transform* parent = object->parent->GetComponent<Transform>();

		float cosValue = cosf(ToRadian(parent->GetWorldRotation().z));
		float sinValue = sinf(ToRadian(parent->GetWorldRotation().z));

		pos.x *= parent->GetWorldScale().x;
		pos.y *= parent->GetWorldScale().y;

		float rx = pos.x;
		float ry = pos.y;

		pos.x = rx * cosValue - ry * sinValue;
		pos.y = rx * sinValue + ry * cosValue;

		pos += parent->GetWorldPosition2D();
	}
	return Vector2(pos.x, pos.y);
}

Vector3 Transform::GetWorldRotation()
{
	return object->parent != nullptr ? rotation + object->parent->GetComponent<Transform>()->GetWorldRotation() : rotation;
}

Vector3 Transform::GetWorldScale()
{
	if (object->parent != nullptr)
	{
		Vector3 sca = scale;
		sca.x *= object->parent->GetComponent<Transform>()->GetWorldScale().x;
		sca.y *= object->parent->GetComponent<Transform>()->GetWorldScale().y;
		sca.z *= object->parent->GetComponent<Transform>()->GetWorldScale().z;

		return sca;
	}
	return scale;
}

Vector3 Transform::Right()
{
	DirectX::XMFLOAT4X4 orientation;

	Vector3 worldRotation = GetWorldRotation() * 0.01745f;
	DirectX::XMStoreFloat4x4(&orientation, DirectX::XMMatrixRotationRollPitchYaw(worldRotation.y, worldRotation.x, worldRotation.z));
	return { orientation._11, orientation._12,orientation._13 };
}

Vector3 Transform::Up()
{
	DirectX::XMFLOAT4X4 orientation;
	Vector3 worldRotation = GetWorldRotation() * 0.01745f;
	DirectX::XMStoreFloat4x4(&orientation, DirectX::XMMatrixRotationRollPitchYaw(worldRotation.y, worldRotation.x, worldRotation.z));
	return { orientation._21, orientation._22,orientation._23 };
}

Vector3 Transform::Forward()
{
	DirectX::XMFLOAT4X4 orientation;
	Vector3 worldRotation = GetWorldRotation() * 0.01745f;
	DirectX::XMStoreFloat4x4(&orientation, DirectX::XMMatrixRotationRollPitchYaw(worldRotation.y, worldRotation.x, worldRotation.z));
	return { orientation._31, orientation._32,orientation._33 };
}
