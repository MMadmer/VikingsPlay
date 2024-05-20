// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BGCCharacterMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartRunning);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRunning);

/**
 * 
 */
UCLASS()
class BASEGAMECOMPONENTS_API UBGCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UBGCCharacterMovementComponent();

	UPROPERTY(BlueprintAssignable)
	FOnStartRunning OnStartRunning;

	UPROPERTY(BlueprintAssignable)
	FOnStopRunning OnStopRunning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement",
		meta = (ClampMin = "1.1", ClampMax = "10", UIMin = "1.1", UIMax = "10"))
	float RunModifier = 2.0f;

	uint8 bWantsToRun : 1;

	UFUNCTION(BlueprintCallable)
	bool IsMoving() const;

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const;

	virtual float GetMaxSpeed() const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	uint8 bWasRunning : 1;
};
