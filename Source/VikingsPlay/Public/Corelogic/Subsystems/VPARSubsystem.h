// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARBlueprintLibrary.h"
#include "ARTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "VPARSubsystem.generated.h"

class UARPlaneGeometry;
class UARSessionConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSessionChanged,
                                             bool, SessionRunning,
                                             EARSessionStatus, SessionStatus);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerTouchedPlane,
                                             UARPlaneGeometry*, PlaneGeometry,
                                             APlayerController*, PlayerController);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScanActiveChanged, bool, IsScanActive);


/**
 * Contains extended information about current AR session.
 */
UCLASS()
class VIKINGSPLAY_API UVPARSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Called when session status changes(from running to any or from any to running). */
	UPROPERTY(BlueprintAssignable)
	FOnSessionChanged OnSessionChanged;

	/** Called when any player controller touched the plane. */
	UPROPERTY(BlueprintAssignable, meta=(DeprecatedProperty))
	FOnPlayerTouchedPlane OnPlayerTouchedPlane;

	/** Called when scanning status changes(not a session). */
	UPROPERTY(BlueprintAssignable)
	FOnScanActiveChanged OnScanActiveChanged;

	// FTickableGameObject implementation Begin
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FVPARSubsystem, STATGROUP_Tickables);
	}

	virtual void Tick(float DeltaTime) override;
	// FTickableGameObject implementation End

	/** Get current AR session status. */
	UFUNCTION(BlueprintPure, meta=(Keywords="Get AR Session Status"))
	static FARSessionStatus GetARSessionStatus() { return UARBlueprintLibrary::GetARSessionStatus(); }

	/** Get current AR session config. */
	UFUNCTION(BlueprintPure, meta=(Keywords="Get AR Session Config"))
	static UARSessionConfig* GetARSessionConfig() { return UARBlueprintLibrary::GetSessionConfig(); }

	/** Get is scan active. */
	UFUNCTION(BlueprintPure, meta=(Keywords="Get Is Scan Active"))
	bool GetIsScanActive() const { return bIsScanActive; }

	/** Set scanning status(AR session must be running to scan). */
	UFUNCTION(BlueprintCallable, meta=(Keywords="Set Is Scan Active"))
	void SetIsScanActive(const bool Active);

	/** Returns nearest plane(or nullptr) under target screen coordinates. */
	UFUNCTION(BlueprintCallable, meta=(Keywords="Get Nearest Plane By Line Trace"))
	static UARPlaneGeometry* GetNearestPlaneByLineTrace(const FVector2D& ScreenPoint);

	/** Returns all found planes. */
	UFUNCTION(BlueprintCallable, meta=(Keywords="Get Found Planes"))
	static TArray<UARPlaneGeometry*> GetFoundPlanes();

protected:
	/** Return current AR session is running or not. */
	UPROPERTY(BlueprintReadOnly)
	bool bSessionIsRunning = false;

	/** Get scanning status(AR session must be running to scan). */
	UPROPERTY(BlueprintReadOnly)
	bool bIsScanActive = false;

	void UpdateSessionStatus();
};
