#pragma once
#include "../../AI/State.h"
#include "Bee.h"

namespace BeeState
{
    // ----- ペアを探す -----
    class SearchState : public State<Bee>
    {
    public:
        SearchState(Bee* bee) : State(bee, "SearchState") {}
        ~SearchState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ----- 花に向かって移動する -----
    class MoveToFlowerState : public State<Bee>
    {
    public:
        MoveToFlowerState(Bee* bee) : State(bee, "MoveToFlowerState") {}
        ~MoveToFlowerState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ----- 蜜を採取する -----
    class CollectState : public State<Bee>
    {
    public:
        CollectState(Bee* bee) : State(bee, "CollectState") {}
        ~CollectState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ----- 置く場所に移動 -----
    class MoveToStoragePlaceState : public State<Bee>
    {
    public:
        MoveToStoragePlaceState(Bee* bee) : State(bee, "MoveToStoragePlaceState") {}
        ~MoveToStoragePlaceState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        DirectX::XMFLOAT3 targetPos = {};
    };
}