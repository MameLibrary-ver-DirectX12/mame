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
        // �ړ�����
        Move(elapsedTime);

        // ��]����
        Rotate(elapsedTime);
    }

    // --- �I���� ---
    void MoveState::Finalize()
    {
    }

    // --- �ړ� ---
    void MoveState::Move(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float ax = gamePad.GetAxisLX();
        float ay = gamePad.GetAxisLY();

        // �ړ�����
        float speed = 10.0f * elapsedTime;
        float maxSpeed = 18.0f * elapsedTime;

        DirectX::XMFLOAT3 velocity = owner_->GetVelocity();
        velocity.x += speed * ay;
        velocity.z += speed * -ax;

        if (velocity.x >= maxSpeed) velocity.x = maxSpeed;
        if (velocity.z >= maxSpeed) velocity.z = maxSpeed;
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

    // --- ��] ---
    void MoveState::Rotate(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        stick_.x = gamePad.GetAxisLX();
        stick_.y = gamePad.GetAxisLY();
        float length = sqrtf(stick_.x * stick_.x + stick_.y * stick_.y);

        // �L�[�{�[�h���͂��X�e�B�b�N�̒l�ɕϊ�
        InputMove();

        if (stick_.x != 0 || stick_.y != 0)
        {
            stick_.x /= length;
            stick_.y /= length;

            DirectX::XMFLOAT3 frontVec = owner_->GetTransform()->CalcForward();
            float dot = frontVec.x * stick_.x + frontVec.z * stick_.y;

            float cross = frontVec.z * stick_.x - frontVec.x * stick_.y;

            float speed = 10.0f * elapsedTime;

            float rotY = (cross > 0.0f) ? acosf(dot) * speed : -acosf(dot) * speed;
            owner_->GetTransform()->SetRotationY(rotY + owner_->GetTransform()->GetRotationY());
            //owner_->GetTransform()->SetRotationY(rotY + owner_->GetTransform()->GetRotationY());
        }
    }

    // --- �L�[�{�[�h���͂��X�e�B�b�N�̒l�ɕϊ� ---
    void MoveState::InputMove()
    {
        // �L�[�{�[�h���͂ł���]����悤�ɂ���
        GamePad& gamePad = Input::Instance().GetGamePad();

        // ���͂�����Βl��ω�������
        if (GetAsyncKeyState('A'))
        {
            stick_.x = -1;
        }
        else if (GetAsyncKeyState('D'))
        {
            stick_.x = 1;
        }

        if (GetAsyncKeyState('S'))
        {
            if (GetAsyncKeyState('A'))
            {
                stick_.x = -0.5;
                stick_.y = -0.5;
            }
            else if (GetAsyncKeyState('D'))
            {
                stick_.x = 0.5;
                stick_.y = -0.5;
            }
            else
            {
                stick_.y = -1;
            }

        }
        else if (GetAsyncKeyState('W'))
        {
            if (GetAsyncKeyState('A'))
            {
                stick_.x = -0.5;
                stick_.y = 0.5;
            }
            else if (GetAsyncKeyState('D'))
            {
                stick_.x = 0.5;
                stick_.y = 0.5;
            }
            else
            {
                stick_.y = 1;
            }
        }

    }
}