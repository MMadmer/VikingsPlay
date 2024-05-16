// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VPPlayer.generated.h"

class UCameraComponent;

UCLASS()
class VIKINGSPLAY_API AVPPlayer : public APawn
{
	GENERATED_BODY()

public:
	AVPPlayer();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Player AR camera. */
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComponent;

protected:
	virtual void BeginPlay() override;
};
