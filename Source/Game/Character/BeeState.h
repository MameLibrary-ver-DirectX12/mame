#pragma once
#include "../../AI/State.h"
#include "Bee.h"

namespace BeeState
{
    // ----- �y�A��T�� -----
    class SearchState : public State<Bee>
    {
    public:
        SearchState(Bee* bee) : State(bee, "SearchState") {}
        ~SearchState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ----- �ԂɌ������Ĉړ����� -----
    class MoveToFlowerState : public State<Bee>
    {
    public:
        MoveToFlowerState(Bee* bee) : State(bee, "MoveToFlowerState") {}
        ~MoveToFlowerState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ----- �����̎悷�� -----
    class CollectState : public State<Bee>
    {
    public:
        CollectState(Bee* bee) : State(bee, "CollectState") {}
        ~CollectState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };

    // ----- �u���ꏊ�Ɉړ� -----
    class MoveToStoragePlaceState : public State<Bee>
    {
    public:
        MoveToStoragePlaceState(Bee* bee) : State(bee, "MoveToStoragePlaceState") {}
        ~MoveToStoragePlaceState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}