// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIKINGSPLAY_API AVPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVPPlayerController();

	/** Player controller touch event. */
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force,
	                        FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;

private:
	const float TouchTraceLength = 10000.0f;
};
