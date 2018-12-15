#include "MyCharacterComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Component/TransformBus.h>

using namespace AZ;
using namespace MultiplayerCharacter;

void MyCharacterComponent::Reflect( AZ::ReflectContext* reflection )
{
    if ( auto serializationContext = azrtti_cast<AZ::SerializeContext*>( reflection ) )
    {
        serializationContext->Class<MyCharacterComponent, Component>()
            ->Version( 1 );

        if ( auto editContext = serializationContext->GetEditContext() )
        {
            editContext->Class<MyCharacterComponent>( "MyCharacterComponent",
                "[Network-friendly implementation of a character for PhysX]" )
                ->ClassElement( AZ::Edit::ClassElements::EditorData, "" )
                ->Attribute( AZ::Edit::Attributes::Category, "MultiplayerCharacter" )
                ->Attribute( AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC( "Game", 0x232b318c ) );
        }
    }
}

void MyCharacterComponent::Init()
{
    m_desiredDirection = AZ::Vector3::CreateZero();
}

void MyCharacterComponent::Activate()
{
    TickBus::Handler::BusConnect();
    MyCharacterBus::Handler::BusConnect( GetEntityId() );
}

void MyCharacterComponent::Deactivate()
{
    TickBus::Handler::BusDisconnect();
    MyCharacterBus::Handler::BusDisconnect();
}

void MyCharacterComponent::PhysicalMove( const AZ::Vector3& direction )
{
}

void MyCharacterComponent::RequestVelocity( const AZ::Vector3& direction )
{
    m_desiredDirection = direction;
}

void MyCharacterComponent::OnTick( float deltaTime, ScriptTimePoint time )
{
    KeepUpRight();

    if ( !m_desiredDirection.IsZero() )
    {
    }
}

void MyCharacterComponent::KeepUpRight()
{
    AZ::Vector3 localRotation;
    AZ::TransformBus::EventResult(localRotation, GetEntityId(), &TransformBus::Events::GetLocalRotation);
    localRotation.SetX( 0.f );
    localRotation.SetY( 0.f );

    AZ::TransformBus::Event(GetEntityId(), &TransformBus::Events::SetLocalRotation, localRotation);
}
