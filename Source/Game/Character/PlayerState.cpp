#include "PlayerState.h"
#include "../../Input/Input.h"

// ----- IdleState -----
namespace PlayerState
{
    // --- 初期化 ---
    void IdleState::Initialize()
    {
    }

    // --- 更新 ---
    void IdleState::Update(const float& elapsedTime)
    {
    }

    // --- 終了化 ---
    void IdleState::Finalize()
    {
    }
}

// ----- MoveState -----
namespace PlayerState
{
    // --- 初期化 ---
    void MoveState::Initialize()
    {
    }

    // --- 更新 ---
    void MoveState::Update(const float& elapsedTime)
    {
        // 移動処理
        Move(elapsedTime);

        // 回転処理
        Rotate(elapsedTime);
    }

    // --- 終了化 ---
    void MoveState::Finalize()
    {
    }

    // --- 移動 ---
    void MoveState::Move(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        float ax = gamePad.GetAxisLX();
        float ay = gamePad.GetAxisLY();

        // 移動処理
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

    // --- 回転 ---
    void MoveState::Rotate(const float& elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        stick_.x = gamePad.GetAxisLX();
        stick_.y = gamePad.GetAxisLY();
        float length = sqrtf(stick_.x * stick_.x + stick_.y * stick_.y);

        // キーボード入力をスティックの値に変換
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

    // --- キーボード入力をスティックの値に変換 ---
    void MoveState::InputMove()
    {
        // キーボード入力でも回転するようにする
        GamePad& gamePad = Input::Instance().GetGamePad();

        // 入力があれば値を変化させる
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