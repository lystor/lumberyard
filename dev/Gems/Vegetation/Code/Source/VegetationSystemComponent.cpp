/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#include "Vegetation_precompiled.h"
#include "VegetationSystemComponent.h"

#include <AzCore/RTTI/BehaviorContext.h>

#include <GradientSignal/ImageSettings.h>
#include <GradientSignal/ImageAsset.h>
#include <Vegetation/DescriptorListAsset.h>
#include <Vegetation/AreaComponentBase.h>
#include <AZFramework/Asset/GenericAssetHandler.h>

#include <Vegetation/EBuses/FilterRequestBus.h>

#include <CrySystemBus.h>

namespace Vegetation
{
    namespace Details
    {
        AzFramework::GenericAssetHandler<DescriptorListAsset>* s_vegetationDescriptorListAssetHandler = nullptr;

        void RegisterAssethandlers()
        {
            s_vegetationDescriptorListAssetHandler = aznew AzFramework::GenericAssetHandler<DescriptorListAsset>("Vegetation Descriptor List", "Other", "vegdescriptorlist");
            s_vegetationDescriptorListAssetHandler->Register();
        }

        void UnregisterAssethandlers()
        {
            if (s_vegetationDescriptorListAssetHandler)
            {
                s_vegetationDescriptorListAssetHandler->Unregister();
                delete s_vegetationDescriptorListAssetHandler;
                s_vegetationDescriptorListAssetHandler = nullptr;
            }
        }
    }

    void VegetationSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC("VegetationSystemService", 0xa2322728));
    }

    void VegetationSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC("VegetationSystemService", 0xa2322728));
    }

    void VegetationSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC("VegetationAreaSystemService", 0x36da2b62));
        services.push_back(AZ_CRC("VegetationInstanceSystemService", 0x823a6007));
        services.push_back(AZ_CRC("SurfaceDataProviderService", 0xfe9fb95e));
    }

    void VegetationSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        Descriptor::Reflect(context);
        AreaConfig::Reflect(context);
        AreaComponentBase::Reflect(context);
        DescriptorListAsset::Reflect(context);

        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<VegetationSystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* editContext = serialize->GetEditContext())
            {
                editContext->Class<VegetationSystemComponent>("Vegetation System", "Reflects types and defines required services for dynamic vegetation systems to function")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Vegetation")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System", 0xc94d118b))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<FilterRequestBus>("FilterRequestBus")
                ->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::Preview)
                ->Attribute(AZ::Script::Attributes::Category, "Vegetation")
                ->Event("GetFilterStage", &FilterRequestBus::Events::GetFilterStage)
                ->Event("SetFilterStage", &FilterRequestBus::Events::SetFilterStage)
                ->VirtualProperty("FilterStage", "GetFilterStage", "SetFilterStage");
                ;
        }
    }

    VegetationSystemComponent::VegetationSystemComponent()
    {
    }

    VegetationSystemComponent::~VegetationSystemComponent()
    {
    }

    void VegetationSystemComponent::Activate()
    {
        Details::RegisterAssethandlers();
    }

    void VegetationSystemComponent::Deactivate()
    {
        Details::UnregisterAssethandlers();
    }

}