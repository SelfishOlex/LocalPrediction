#include "MyCharacterComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Component/TransformBus.h>
#include <AzFramework/Physics/SystemBus.h>
#include <AzFramework/Physics/ShapeConfiguration.h>
#include <PhysX/PhysXColliderComponentBus.h>

using namespace AZ;
using namespace MultiplayerCharacter;

#pragma optimize("", off)

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
    m_desiredDirectionAndSpeed = AZ::Vector3::CreateZero();
    m_gravity = AZ::Vector3::CreateAxisZ( -9.81f );
}

void MyCharacterComponent::Activate()
{
#if defined(DEDICATED_SERVER)
    TickBus::Handler::BusConnect();
    MyCharacterBus::Handler::BusConnect( GetEntityId() );

    Physics::SystemRequestBus::BroadcastResult( m_physXWorld, &Physics::SystemRequestBus::Events::GetDefaultWorld );
    AZ_Assert( m_physXWorld, "Didn't get PhysX world" );
#endif
}

void MyCharacterComponent::Deactivate()
{
    TickBus::Handler::BusDisconnect();
    MyCharacterBus::Handler::BusDisconnect();

    m_physXWorld = nullptr;
}

void MyCharacterComponent::PhysicalMove( const AZ::Vector3& direction, float deltaTime )
{
    if ( direction.IsZero() )
    {
        ApplyGravity( deltaTime );
    }
    else
    {
        PhysicalMoveWithoutGravity( direction, deltaTime );
    }
}

void MyCharacterComponent::RequestVelocity( const AZ::Vector3& direction )
{
    m_desiredDirectionAndSpeed = direction;
}

void MyCharacterComponent::OnTick( float deltaTime, ScriptTimePoint time )
{
    KeepUpRight();
    PhysicalMove( m_desiredDirectionAndSpeed, deltaTime );
}

void MyCharacterComponent::KeepUpRight()
{
    AZ::Vector3 localRotation;
    AZ::TransformBus::EventResult( localRotation, GetEntityId(), &TransformBus::Events::GetLocalRotation );
    localRotation.SetX( 0.f );
    localRotation.SetY( 0.f );

    AZ::TransformBus::Event( GetEntityId(), &TransformBus::Events::SetLocalRotation, localRotation );
}

void MyCharacterComponent::PhysicalMoveWithoutGravity(const AZ::Vector3& direction, float deltaTime)
{
    const AZ::Vector3 moveAmount = direction * deltaTime;

    Physics::RayCastRequest request;
    Physics::RayCastResult result;

    // set the start
    AZ::Vector3 startLocation;
    AZ::TransformBus::EventResult( startLocation, GetEntityId(), &AZ::TransformBus::Events::GetWorldTranslation );
    request.m_start = startLocation;

    request.m_start.SetZ( request.m_start.GetZ() + 0.5f );

    /*AZ_Printf( "LocalPrediction", "Character @ (%f %f %f)",
        float( request.m_start.GetX() ), float( request.m_start.GetY() ), float( request.m_start.GetZ() ) );*/

    request.m_dir = direction.GetNormalized();
    request.m_time = direction.GetLength() * deltaTime;

    m_physXWorld->RayCast( request, result );

    if ( result.m_hits.empty() )
    {
        const AZ::Vector3 end = startLocation + request.m_dir * request.m_time;

        // no collision, move on forward
        AZ::TransformBus::Event( GetEntityId(), &AZ::TransformBus::Events::SetWorldTranslation, end );

        AZ_Printf( "LocalPrediction", "Character freely moving to (%f %f %f)", float( end.GetX() ), float( end.GetY() ), float( end.GetZ() ) );
    }
    else if ( result.m_hits[0].m_hitTime > 0 ) // hack around PhysX Gem implementation that returns bad hits
    {
        // move to the collision point
        const AZ::Vector3 locationAtCollision = startLocation + AZStd::GetMax( result.m_hits[0].m_hitTime - m_characterSize, 0.f) * request.m_dir;

        AZ::TransformBus::Event( GetEntityId(), &AZ::TransformBus::Events::SetWorldTranslation, locationAtCollision );

        /*AZ_Printf( "LocalPrediction", "Character bumped into something, moved to (%f %f %f)",
            float( locationAtCollision.GetX() ), float( locationAtCollision.GetY() ), float( locationAtCollision.GetZ() ) );*/
    }
}

void MyCharacterComponent::ApplyGravity( float deltaTime )
{
    PhysicalMove( m_gravity, deltaTime );
}
