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

#include "UiInteractableComponent.h"
#include <AzCore/Math/Vector2.h>
#include <LyShine/Bus/UiScrollBarBus.h>
#include <LyShine/Bus/UiScrollerBus.h>
#include <LyShine/Bus/UiInitializationBus.h>
#include <LyShine/Bus/UiScrollableBus.h>
#include <LyShine/Bus/UiTransformBus.h>
#include <LyShine/UiComponentTypes.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
class UiScrollBarComponent
    : public UiInteractableComponent
    , public UiScrollBarBus::Handler
    , public UiScrollerBus::Handler
    , public UiInitializationBus::Handler
    , public UiScrollableToScrollerNotificationBus::Handler
    , public UiTransformChangeNotificationBus::Handler
{
public: // member functions

    AZ_COMPONENT(UiScrollBarComponent, LyShine::UiScrollBarComponentUuid, AZ::Component);

    UiScrollBarComponent();
    ~UiScrollBarComponent() override;

    // UiScrollBarInterface
    float GetHandleSize() override;
    void SetHandleSize(float size) override;
    float GetMinHandlePixelSize() override;
    void SetMinHandlePixelSize(float size) override;
    AZ::EntityId GetHandleEntity() override;
    void SetHandleEntity(AZ::EntityId entityId) override;
    // ~UiScrollBarInterface

    // UiScrollerInterface
    Orientation GetOrientation() override;
    void SetOrientation(Orientation orientation);
    AZ::EntityId GetScrollableEntity() override;
    void SetScrollableEntity(AZ::EntityId entityId) override;
    float GetValue() override;
    void SetValue(float value) override;
    ValueChangeCallback GetValueChangingCallback() override;
    void SetValueChangingCallback(ValueChangeCallback onChange) override;
    const LyShine::ActionName& GetValueChangingActionName() override;
    void SetValueChangingActionName(const LyShine::ActionName& actionName) override;
    ValueChangeCallback GetValueChangedCallback() override;
    void SetValueChangedCallback(ValueChangeCallback onChange) override;
    const LyShine::ActionName& GetValueChangedActionName() override;
    void SetValueChangedActionName(const LyShine::ActionName& actionName) override;
    // ~UiScrollerInterface

    // UiScrollableToScrollerNotifications
    void OnValueChangingByScrollable(AZ::Vector2 value) override;
    void OnValueChangedByScrollable(AZ::Vector2 value) override;
    void OnScrollableParentToContentRatioChanged(AZ::Vector2 parentToContentRatio) override;
    // ~UiScrollableToScrollerNotifications

    // UiInitializationInterface
    void InGamePostActivate() override;
    // ~UiInitializationInterface

    // UiInteractableInterface
    bool HandlePressed(AZ::Vector2 point, bool& shouldStayActive) override;
    bool HandleReleased(AZ::Vector2 point) override;
    bool HandleEnterPressed(bool& shouldStayActive) override;
    bool HandleKeyInput(EKeyId keyId, int modifiers) override;
    void InputPositionUpdate(AZ::Vector2 point) override;
    bool DoesSupportDragHandOff(AZ::Vector2 startPoint) override;
    bool OfferDragHandOff(AZ::EntityId currentActiveInteractable, AZ::Vector2 startPoint, AZ::Vector2 currentPoint, float dragThreshold) override;
    void LostActiveStatus() override;
    // ~UiInteractableInterface

    // UiTransformChangeNotificationBus
    void OnCanvasSpaceRectChanged(AZ::EntityId entityId, const UiTransformInterface::Rect& oldRect, const UiTransformInterface::Rect& newRect) override;
    // ~UiTransformChangeNotificationBus

protected: // member functions

    // AZ::Component
    void Activate() override;
    void Deactivate() override;
    // ~AZ::Component

    UiInteractableStatesInterface::State ComputeInteractableState() override;

    static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("UiInteractableService", 0x1d474c98));
        provided.push_back(AZ_CRC("UiNavigationService"));
        provided.push_back(AZ_CRC("UiStateActionsService"));
    }

    static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("UiInteractableService", 0x1d474c98));
        incompatible.push_back(AZ_CRC("UiNavigationService"));
        incompatible.push_back(AZ_CRC("UiStateActionsService"));
    }

    static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC("UiElementService", 0x3dca7ad4));
        required.push_back(AZ_CRC("UiTransformService", 0x3a838e34));
    }

    static void Reflect(AZ::ReflectContext* context);

private: // types
    enum class LocRelativeToHandle
    {
        BeforeHandle,
        OnHandle,
        AfterHandle
    };

private: // member functions

    AZ_DISABLE_COPY_MOVE(UiScrollBarComponent);

    using EntityComboBoxVec = AZStd::vector< AZStd::pair< AZ::EntityId, AZStd::string > >;
    EntityComboBoxVec PopulateChildEntityList();

    float GetValidDragDistanceInPixels(AZ::Vector2 startPoint, AZ::Vector2 endPoint);
    bool CheckForDragOrHandOffToParent(AZ::EntityId currentActiveInteractable, AZ::Vector2 startPoint, AZ::Vector2 currentPoint, float childDragThreshold, bool& handOffDone);

    void DoSetValue(float value);

    void DoChangedActions();
    void DoChangingActions();

    void NotifyListenersOnValueChanged();
    void NotifyListenersOnValueChanging();

    void NotifyScrollableOnValueChanged();
    void NotifyScrollableOnValueChanging();

    LocRelativeToHandle GetLocationRelativeToHandle(const AZ::Vector2& point);
    float GetHandleParentLength();
    float GetPosAlongAxis(AZ::Vector2 point);
    bool MoveHandle(LocRelativeToHandle pointLoc);

private: // data

    float m_value;
    float m_handleSize;
    float m_minHandlePixelSize;
    Orientation m_orientation;

    float m_displayedHandleSize;

    bool m_isDragging;
    bool m_isActive; // true when interactable can be manipulated by key input

    float m_pressedValue;
    float m_pressedPosAlongAxis;
    bool m_pressedOnHandle;
    float m_lastMoveTime;
    float m_moveDelayTime;

    ValueChangeCallback m_onValueChanged;
    ValueChangeCallback m_onValueChanging;

    LyShine::ActionName m_valueChangedActionName;
    LyShine::ActionName m_valueChangingActionName;

    AZ::EntityId m_handleEntity;
    AZ::EntityId m_scrollableEntity;
};
