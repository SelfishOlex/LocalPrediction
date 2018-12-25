#include "LocalPredictionComponent.h"
#include <AzFramework/Network/NetworkContext.h>
#include <AzCore/Serialization/EditContext.h>

using namespace AZ;
using namespace AzFramework;
using namespace GridMate;
using namespace MultiplayerCharacter;

void LocalPredictionComponent::Reflect(AZ::ReflectContext* reflection)
{
    if ( auto serializationContext = azrtti_cast<AZ::SerializeContext*>( reflection ) )
    {
        serializationContext->Class<LocalPredictionComponent, Component>()
                            ->Version( 1 );

        if ( auto editContext = serializationContext->GetEditContext() )
        {
            editContext->Class<LocalPredictionComponent>( "LocalPredictionComponent",
                                   "[local prediction logic]" )
                       ->ClassElement( AZ::Edit::ClassElements::EditorData, "" )
                       ->Attribute( AZ::Edit::Attributes::Category, "MultiplayerCharacter" )
                       ->Attribute( AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c) );
        }
    }

    if(auto* networkContext = azrtti_cast<NetworkContext*>(reflection))
    {
        networkContext->Class<LocalPredictionComponent>()
            ;
    }
}

void LocalPredictionComponent::Init()
{
}

void LocalPredictionComponent::Activate()
{
    LocalPlayerControlsRequestBus::Handler::BusConnect( GetEntityId() );
}

void LocalPredictionComponent::Deactivate()
{
    LocalPlayerControlsRequestBus::Handler::BusDisconnect();
}

void LocalPredictionComponent::MoveForward(ActionState state)
{
}

void LocalPredictionComponent::MoveBackward(ActionState state)
{
}

void LocalPredictionComponent::StrafeLeft(ActionState state)
{
}

void LocalPredictionComponent::StrafeRight(ActionState state)
{
}
