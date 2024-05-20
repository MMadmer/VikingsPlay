// Fill out your copyright notice in the Description page of Project Settings.


#include "Corelogic/Subsystems/VPARSubsystem.h"


void UVPARSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSessionStatus();

	if (bIsScanActive)
	{
	}
}

void UVPARSubsystem::SetIsScanActive(const bool Active)
{
	if (bIsScanActive != Active)
	{
		bIsScanActive = Active;
		OnScanActiveChanged.Broadcast(bIsScanActive);
	}
}

UARPlaneGeometry* UVPARSubsystem::GetNearestPlaneByLineTrace(const FVector2D& ScreenPoint)
{
	TArray<FARTraceResult> TrackedObjects = UARBlueprintLibrary::LineTraceTrackedObjects(
		ScreenPoint, false, true, true, true);

	return Cast<UARPlaneGeometry>(TrackedObjects.IsValidIndex(0)
		                              ? TrackedObjects[0].GetTrackedGeometry()
		                              : nullptr);
}

TArray<UARPlaneGeometry*> UVPARSubsystem::GetFoundPlanes()
{
	TArray<UARPlaneGeometry*> PlaneGeometries;
	TArray<UARTrackedGeometry*> TrackedGeometries = UARBlueprintLibrary::GetAllGeometriesByClass(
		UARPlaneGeometry::StaticClass());

	for (const auto TrackedGeometry : TrackedGeometries)
	{
		UARPlaneGeometry* PlaneGeometry = Cast<UARPlaneGeometry>(TrackedGeometry);
		if (!PlaneGeometry) continue;

		PlaneGeometries.Add(PlaneGeometry);
	}

	return PlaneGeometries;
}

void UVPARSubsystem::UpdateSessionStatus()
{
	const EARSessionStatus CurrentSessionStatus = GetARSessionStatus().Status;

	if (CurrentSessionStatus == EARSessionStatus::Running)
	{
		if (!bSessionIsRunning)
		{
			bSessionIsRunning = true;
			OnSessionChanged.Broadcast(bSessionIsRunning, CurrentSessionStatus);
		}
	}
	else
	{
		if (bSessionIsRunning)
		{
			bSessionIsRunning = false;
			OnSessionChanged.Broadcast(bSessionIsRunning, CurrentSessionStatus);
		}
	}
}
