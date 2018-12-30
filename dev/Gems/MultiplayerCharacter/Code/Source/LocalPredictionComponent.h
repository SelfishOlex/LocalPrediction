#pragma once

#include <AzCore/Component/Component.h>
#include <AzFramework/Network/NetBindable.h>
#include <MultiplayerCharacter/PlayerControlsRequestBus.h>

namespace MultiplayerCharacter
{
    /**
     * \brief
     */
    class LocalPredictionComponent
        : public AZ::Component
        , public AzFramework::NetBindable
        , public LocalPlayerControlsRequestBus::Handler
    {
    public:
        AZ_COMPONENT( LocalPredictionComponent, "{F8941E15-43F2-47B8-B3C0-080BDFDC179D}", NetBindable );

        ~LocalPredictionComponent() override = default;

        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
        {
            required.push_back(AZ_CRC("TransformService"));
        }

        static void Reflect( AZ::ReflectContext* reflection );

        void Init() override;
        void Activate() override;
        void Deactivate() override;

    protected:
        // LocalPlayerControlsRequestBus
        void MoveForward(ActionState state) override;
        void MoveBackward(ActionState state) override;
        void StrafeLeft(ActionState state) override;
        void StrafeRight(ActionState state) override;

    private:
        // network c-tor

        AZ::Vector3 m_spawnLocation;

        const AZ::Vector3& GetSpawnLocation();
        void SetSpawnLocation(const AZ::Vector3& initialLocation);
    };
}
