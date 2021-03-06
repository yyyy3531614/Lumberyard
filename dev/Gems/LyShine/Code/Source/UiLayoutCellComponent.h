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
#pragma once

#include <LyShine/Bus/UiLayoutCellBus.h>
#include <LyShine/UiComponentTypes.h>

#include <AzCore/Component/Component.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//! This component allows the default layout cell properties to be overridden
class UiLayoutCellComponent
    : public AZ::Component
    , public UiLayoutCellBus::Handler
{
public: // member functions

    AZ_COMPONENT(UiLayoutCellComponent, LyShine::UiLayoutCellComponentUuid, AZ::Component);

    UiLayoutCellComponent();
    ~UiLayoutCellComponent() override;

    // UiLayoutCellInterface
    float GetMinWidth() override;
    void SetMinWidth(float width) override;
    float GetMinHeight() override;
    void SetMinHeight(float height) override;
    float GetTargetWidth() override;
    void SetTargetWidth(float width) override;
    float GetTargetHeight() override;
    void SetTargetHeight(float height) override;
    float GetExtraWidthRatio() override;
    void SetExtraWidthRatio(float width) override;
    float GetExtraHeightRatio() override;
    void SetExtraHeightRatio(float height) override;
    // ~UiLayoutCellInterface

public:  // static member functions

    static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("UiLayoutCellService", 0x67ba28f2));
    }

    static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("UiLayoutCellService", 0x67ba28f2));
    }

    static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC("UiElementService", 0x3dca7ad4));
        required.push_back(AZ_CRC("UiTransformService", 0x3a838e34));
    }

    static void Reflect(AZ::ReflectContext* context);

protected: // member functions

    // AZ::Component
    void Activate() override;
    void Deactivate() override;
    // ~AZ::Component

    AZ_DISABLE_COPY_MOVE(UiLayoutCellComponent);

    //! Invalidate the parent's layout. Called when a layout cell property has changed
    void InvalidateParentLayout();

protected: // data

    //! Whether the minimum width has been overridden
    bool m_minWidthOverridden;

    //! The minimum width
    float m_minWidth;

    //! Whether the minimum height has been overridden
    bool m_minHeightOverridden;

    //! The minimum height
    float m_minHeight;

    //! Whether the target width has been overridden
    bool m_targetWidthOverridden;

    //! The target width
    float m_targetWidth;

    //! Whether the target height has been overridden
    bool m_targetHeightOverridden;

    //! The target height
    float m_targetHeight;

    //! Whether the extra width ratio has been overridden
    bool m_extraWidthRatioOverridden;

    //! The extra width ratio
    float m_extraWidthRatio;

    //! Whether the extra height ratio has been overridden
    bool m_extraHeightRatioOverridden;

    //! The extra height ratio
    float m_extraHeightRatio;
};
