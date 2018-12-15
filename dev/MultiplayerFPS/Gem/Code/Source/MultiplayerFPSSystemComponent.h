
#pragma once

#include <AzCore/Component/Component.h>

#include <MultiplayerFPS/MultiplayerFPSBus.h>

namespace MultiplayerFPS
{
    class MultiplayerFPSSystemComponent
        : public AZ::Component
        , protected MultiplayerFPSRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MultiplayerFPSSystemComponent, "{0D17EEA2-DE79-48EC-AEDA-D8CBB5334764}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        ////////////////////////////////////////////////////////////////////////
        // MultiplayerFPSRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
