#include "LocalPredictionComponent.h"
#include <AzFramework/Network/NetworkContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Component/TransformBus.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>

#pragma optimize("", off)

using namespace AZ;
using namespace AzFramework;
using namespace GridMate;
using namespace MultiplayerCharacter;

void LocalPredictionComponent::Reflect(AZ::ReflectContext* reflection)
{
    if (auto serializationContext = azrtti_cast<AZ::SerializeContext*>(reflection))
    {
        serializationContext->Class<LocalPredictionComponent, Component>()
            ->Field("Spawn Location", &LocalPredictionComponent::m_spawnLocation)
            ->Version(1);

        if (auto editContext = serializationContext->GetEditContext())
        {
            editContext->Class<LocalPredictionComponent>("LocalPredictionComponent",
                "[local prediction logic]")
                ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                ->Attribute(AZ::Edit::Attributes::Category, "MultiplayerCharacter")
                ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c));
        }
    }

    if (auto* networkContext = azrtti_cast<NetworkContext*>(reflection))
    {
        networkContext->Class<LocalPredictionComponent>()
            ->CtorData("Spawn Location", &LocalPredictionComponent::GetSpawnLocation, &LocalPredictionComponent::SetSpawnLocation)
            ;
    }
}

void LocalPredictionComponent::Init()
{
}

void LocalPredictionComponent::Activate()
{
    LocalPlayerControlsRequestBus::Handler::BusConnect(GetEntityId());

    if (AzFramework::NetQuery::IsEntityAuthoritative( GetEntityId() ))
    {
        // on the server, save the spawn location
        AZ::TransformBus::EventResult(m_spawnLocation, GetEntityId(), &AZ::TransformBus::Events::GetWorldTranslation);
        AZ_Printf("MultiplayerCharacter", "LocalPredictionComponent GetSpawnLocation %f %f %f",
            m_spawnLocation.GetX().operator float(), m_spawnLocation.GetY().operator float(), m_spawnLocation.GetZ().operator float());
    }
    else
    {
        // on the client, apply the spawn location
        AZ_Printf("MultiplayerCharacter", "LocalPredictionComponent SetSpawnLocation %f %f %f",
            m_spawnLocation.GetX().operator float(), m_spawnLocation.GetY().operator float(), m_spawnLocation.GetZ().operator float());
        AZ::TransformBus::Event(GetEntityId(), &AZ::TransformBus::Events::SetWorldTranslation, m_spawnLocation);
    }
}

void LocalPredictionComponent::Deactivate()
{
    LocalPlayerControlsRequestBus::Handler::BusDisconnect();
}

void LocalPredictionComponent::MoveForward(ActionState state)
{
    AZ_Printf("MultiplayerCharacter", "LocalPredictionComponent MoveForward %d", AZ::u8(state));
}

void LocalPredictionComponent::MoveBackward(ActionState state)
{
    AZ_Printf("MultiplayerCharacter", "LocalPredictionComponent MoveBackward %d", AZ::u8(state));
}

void LocalPredictionComponent::StrafeLeft(ActionState state)
{
    AZ_Printf("MultiplayerCharacter", "LocalPredictionComponent StrafeLeft %d", AZ::u8(state));
}

void LocalPredictionComponent::StrafeRight(ActionState state)
{
    AZ_Printf("MultiplayerCharacter", "LocalPredictionComponent StrafeRight %d", AZ::u8(state));
}

const AZ::Vector3& LocalPredictionComponent::GetSpawnLocation()
{
    return m_spawnLocation;
}

void LocalPredictionComponent::SetSpawnLocation(const AZ::Vector3& initialLocation)
{
    m_spawnLocation = initialLocation;
}
