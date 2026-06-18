#pragma once


#include "Component.h"


namespace NardaEngine
{
    class TransformComponent;
    class RigidBodyComponent;

    class PlayerControllerComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::PlayerController);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void DebugUI() override;
        void Deserialize(const rapidjson::Value& value) override;
        void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue) override;


    private:
        TransformComponent* mTransformComponent = nullptr;
        RigidBodyComponent* mRigidBodyComponent = nullptr;

        float mShiftSpeed = 10.0f;
        float mMoveSpeed = 1.0f;
        float mTurnSpeed = 0.1f;
        float mJumpSpeed = 10.0f;

        // Data for double Jump: speed, jump count, max jumps and was grounded last frame

        float mDoubleJumpSpeed = 8.0f;      // second jump slightly lower than mJumpSpeed (10.0f)
        int mJumpCount = 0;                 // 0=on ground, 1=first jump used, 2=both used
        int mMaxJumps = 2;;                 // maximum jumps before needing to land
        bool mWasGrounded = false;          // ground state from previous frame (unused after timer fix)

        //Grounded detection
        float mGroundedTimer = 0.0f;
        const float mGroundedDelay = 0.1f;
    };
}