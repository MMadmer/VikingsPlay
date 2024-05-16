// Fill out your copyright notice in the Description page of Project Settings.


#include "Corelogic/Player/VPPlayerController.h"

#include "Corelogic/Subsystems/VPARSubsystem.h"

AVPPlayerController::AVPPlayerController()
{
}

bool AVPPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force,
                                     FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	const bool Result = Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);

	int32 ScreenX = 0;
	int32 ScreenY = 0;
	GetViewportSize(ScreenX, ScreenY);

	UVPARSubsystem* ARSubsystem = GetWorld()->GetSubsystem<UVPARSubsystem>();
	if (ARSubsystem && ARSubsystem->GetIsScanActive())
	{
		FARTraceResult PlaneGeometry = ARSubsystem->GetNearestPlaneByLineTrace(FVector2D(ScreenX, ScreenY));
		if (PlaneGeometry.GetTrackedGeometry())
		{
			OnPlaneTouched.Broadcast(PlaneGeometry);
		}
	}
	else
	{
		if (Type == ETouchType::Began || Type == ETouchType::Ended)
		{
			FVector ScreenWorldLocation{};
			FVector ScreenWorldDirection{};

			if (DeprojectScreenPositionToWorld(ScreenX, ScreenY, ScreenWorldLocation, ScreenWorldDirection))
			{
				UWorld* World = GetWorld();
				if (World)
				{
					const FVector StartLocation = ScreenWorldLocation;
					const FVector EndLocation = StartLocation + (ScreenWorldDirection * TouchTraceLength);
					FHitResult HitResult{};

					if (World->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation,
					                                       FCollisionObjectQueryParams::AllObjects))
					{
						switch (Type)
						{
						case ETouchType::Began:
							HitResult.Actor->OnInputTouchBegin.Broadcast(ETouchIndex::Touch1, GetPawn());
							break;
						case ETouchType::Ended:
							HitResult.Actor->OnInputTouchEnd.Broadcast(ETouchIndex::Touch1, GetPawn());
							break;
						default: ;
						}
					}
				}
			}
		}
	}

	return Result;
}
