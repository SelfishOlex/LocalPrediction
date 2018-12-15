#pragma once
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Math/Vector3.h>

namespace MultiplayerCharacter
{
    class MyCharacterInterface
        : public AZ::ComponentBus
    {
    public:
        virtual ~MyCharacterInterface() = default;

        /**
         * \brief Move in the direction until told otherwise
         * \param direction magnitude is used as speed (per second)
         */
        virtual void RequestVelocity(const AZ::Vector3& direction) = 0;

        /**
         * \brief Move immediately physically in the world
         * \param direction magnitude is used to tell how far to move
         */
        virtual void PhysicalMove(const AZ::Vector3& direction) = 0;
    };

    using MyCharacterBus = AZ::EBus<MyCharacterInterface>;
} // namespace MultiplayerCharacter