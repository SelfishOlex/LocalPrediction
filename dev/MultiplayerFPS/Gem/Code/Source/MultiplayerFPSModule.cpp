

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <MultiplayerFPSSystemComponent.h>

namespace MultiplayerFPS
{
    class MultiplayerFPSModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(MultiplayerFPSModule, "{044A03A1-CA11-4463-B766-E31245CACAE6}", AZ::Module);
        AZ_CLASS_ALLOCATOR(MultiplayerFPSModule, AZ::SystemAllocator, 0);

        MultiplayerFPSModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                MultiplayerFPSSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<MultiplayerFPSSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(MultiplayerFPS_f6d12f3a1c9546b4979c80d293543db1, MultiplayerFPS::MultiplayerFPSModule)
