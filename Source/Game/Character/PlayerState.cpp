#include "PlayerState.h"
#include "../../Input/Input.h"

// ----- IdleState -----
namespace PlayerState
{
    // --- ������ ---
    void IdleState::Initialize()
    {
    }

    // --- �X�V ---
    void IdleState::Update(const float& elapsedTime)
    {
    }

    // --- �I���� ---
    void IdleState::Finalize()
    {
    }
}

// ----- MoveState -----
namespace PlayerState
{
    // --- ������ ---
    void MoveState::Initialize()
    {
    }

    // --- �X�V ---
    void MoveState::Update(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float ax = gamePad.GetAxisLX();
        float ay = gamePad.GetAxisLY();
        float length = sqrtf(ax * ax + ay * ay);

        // ��]����
        {
            if (ax != 0 && ay != 0)
            {
                ax /= length;
                ay /= length;

                DirectX::XMFLOAT3 frontVec = owner_->GetTransform()->CalcForward();
                float dot = frontVec.x * ax + frontVec.z * ay;

                float cross = frontVec.z * ax - frontVec.x * ay;

                float speed = 10.0f * elapsedTime;

                float rotY = (cross > 0.0f) ? acosf(dot) * speed : -acosf(dot) * speed;
                owner_->GetTransform()->SetRotationY(rotY + owner_->GetTransform()->GetRotationY());
                //owner_->GetTransform()->SetRotationY(rotY + owner_->GetTransform()->GetRotationY());
            } 
        }

        ax = gamePad.GetAxisLX();
        ay = gamePad.GetAxisLY();

        // �ړ�����
        float speed = 10.0f * elapsedTime;
        float maxSpeed = 18.0f * elapsedTime;

        DirectX::XMFLOAT3 velocity = owner_->GetVelocity();
        velocity.x += speed * ay;
        velocity.z += speed * -ax;

        if (velocity.x >=  maxSpeed) velocity.x =  maxSpeed;
        if (velocity.z >=  maxSpeed) velocity.z =  maxSpeed;
        if (velocity.x <= -maxSpeed) velocity.x = -maxSpeed;
        if (velocity.z <= -maxSpeed) velocity.z = -maxSpeed;

        float masatukeisu = 0.7f;

        if (velocity.x >= 0.001f || velocity.x <= -0.001f)
        {
            float masatu = velocity.x > 0 ? -masatukeisu : masatukeisu;
            masatu *= elapsedTime;

            velocity.x += masatu;
        }
        else
        {
            velocity.x = 0;
        }

        if (velocity.z >= 0.001f || velocity.z <= -0.001f)
        {
            float masatu = velocity.z > 0 ? -masatukeisu : masatukeisu;
            masatu *= elapsedTime;

            velocity.z += masatu;
        }
        else
        {
            velocity.z = 0;
        }

        owner_->GetTransform()->AddPosition(velocity);
        owner_->SetVelocity(velocity);
    }

    // --- �I���� ---
    void MoveState::Finalize()
    {
    }
}