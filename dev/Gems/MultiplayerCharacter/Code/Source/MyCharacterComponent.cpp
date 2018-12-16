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
}

void MyCharacterComponent::Activate()
{
    TickBus::Handler::BusConnect();
    MyCharacterBus::Handler::BusConnect( GetEntityId() );

    Physics::SystemRequestBus::BroadcastResult( m_physXWorld, &Physics::SystemRequestBus::Events::GetDefaultWorld );
    AZ_Assert( m_physXWorld, "Didn't get PhysX world" );
}

void MyCharacterComponent::Deactivate()
{
    TickBus::Handler::BusDisconnect();
    MyCharacterBus::Handler::BusDisconnect();

    m_physXWorld = nullptr;
}

void MyCharacterComponent::PhysicalMove( const AZ::Vector3& direction )
{
    Physics::ShapeCastRequest request;
    Physics::ShapeCastResult result;

    // set the start of the shape cast
    AZ::TransformBus::EventResult( request.m_start, GetEntityId(), &AZ::TransformBus::Events::GetWorldTM );

    AZ_Printf( "LocalPrediction", "Character @ (%f %f %f)",
        float( request.m_start.GetTranslation().GetX() ), float( request.m_start.GetTranslation().GetY() ), float( request.m_start.GetTranslation().GetZ() ) );

    // set the end of the shape cast
    AZ::Transform end = request.m_start;
    const AZ::Vector3 destinationVector = end.GetTranslation() + direction;
    end.SetTranslation( destinationVector );
    request.m_end = end;

    // set the shape
    PhysX::PhysXColliderComponentRequestBus::EventResult( request.m_shapeConfiguration, GetEntityId(), &PhysX::PhysXColliderComponentRequestBus::Events::GetShapeConfigFromEntity );

    m_physXWorld->ShapeCast( request, result );

    if ( result.m_hits.empty() )
    {
        // no collision, move on forward
        AZ::TransformBus::Event( GetEntityId(), &AZ::TransformBus::Events::SetWorldTM, end );

        AZ_Printf( "LocalPrediction", "Character freely moving to (%f %f %f)",
            float( destinationVector.GetX() ), float( destinationVector.GetY() ), float( destinationVector.GetZ() ) );
    }
    else
    {
        // move to the collision point
        AZ::Transform locationAtCollision = request.m_start;
        locationAtCollision.SetTranslation( result.m_hits[0].m_position );

        AZ::TransformBus::Event( GetEntityId(), &AZ::TransformBus::Events::SetWorldTM, locationAtCollision );

        const AZ::Vector3 moveToLocation = result.m_hits[0].m_position;
        AZ_Printf( "LocalPrediction", "Character bumped into something, moved to (%f %f %f)",
            float( moveToLocation.GetX() ), float( moveToLocation.GetY() ), float( moveToLocation.GetZ() ) );
    }
}

void MyCharacterComponent::RequestVelocity( const AZ::Vector3& direction )
{
    m_desiredDirectionAndSpeed = direction;
}

void MyCharacterComponent::OnTick( float deltaTime, ScriptTimePoint time )
{
    KeepUpRight();

    if ( !m_desiredDirectionAndSpeed.IsZero() )
    {
        const AZ::Vector3 moveThisFrame = m_desiredDirectionAndSpeed * deltaTime;
        PhysicalMove( moveThisFrame );
    }
}

void MyCharacterComponent::KeepUpRight()
{
    AZ::Vector3 localRotation;
    AZ::TransformBus::EventResult( localRotation, GetEntityId(), &TransformBus::Events::GetLocalRotation );
    localRotation.SetX( 0.f );
    localRotation.SetY( 0.f );

    AZ::TransformBus::Event( GetEntityId(), &TransformBus::Events::SetLocalRotation, localRotation );
}
