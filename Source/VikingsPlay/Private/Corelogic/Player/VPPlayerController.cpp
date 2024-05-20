// Fill out your copyright notice in the Description page of Project Settings.


#include "Corelogic/Player/VPPlayerController.h"

#include "Corelogic/Subsystems/VPARSubsystem.h"
#include "Interfaces/VPInteraction.h"

AVPPlayerController::AVPPlayerController()
{
}

bool AVPPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force,
                                     FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	const bool Result = Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);

	if (Type == ETouchType::Began || Type == ETouchType::Ended)
	{
		int32 ScreenX = 0;
		int32 ScreenY = 0;
		GetViewportSize(ScreenX, ScreenY);

		FVector ScreenWorldLocation{};
		FVector ScreenWorldDirection{};

		if (DeprojectScreenPositionToWorld(ScreenX, ScreenY, ScreenWorldLocation, ScreenWorldDirection))
		{
			const UWorld* World = GetWorld();

			if (World)
			{
				const UVPARSubsystem* ARSubsystem = World->GetSubsystem<UVPARSubsystem>();

				if (ARSubsystem->GetIsScanActive())
				{
					if (Type == ETouchType::Began)
						if (UARPlaneGeometry* PlaneGeometry = ARSubsystem->GetNearestPlaneByLineTrace(
							FVector2D(ScreenX, ScreenY)))
							ARSubsystem->OnPlayerTouchedPlane.Broadcast(PlaneGeometry, this);
				}
				else
				{
					const FVector StartLocation = ScreenWorldLocation;
					const FVector EndLocation = StartLocation + (ScreenWorldDirection * TouchTraceLength);
					FHitResult HitResult{};
					FCollisionQueryParams Params{};
					Params.bTraceComplex = true;

					if (World->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation,
					                                       FCollisionObjectQueryParams::AllObjects, Params))
					{
						switch (Type)
						{
						case ETouchType::Began:
							HitResult.Actor->OnInputTouchBegin.Broadcast(ETouchIndex::Touch1, GetPawn());

							break;
						case ETouchType::Ended:
							HitResult.Actor->OnInputTouchEnd.Broadcast(ETouchIndex::Touch1, GetPawn());

							if (HitResult.Actor->Implements<UVPInteraction>())
								IVPInteraction::Execute_Interact(
									HitResult.Actor.Get(), GetPawn(), HitResult);

							break;
						default: ;
						}
					}
					else
					{
						if (World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, Params))
						{
							if (Type == ETouchType::Ended)
							{
								if (HitResult.Actor->Implements<UVPInteraction>())
									IVPInteraction::Execute_Interact(
										HitResult.Actor.Get(), GetPawn(), HitResult);
							}
						}
					}
				}
			}
		}
	}

	return Result;
}
