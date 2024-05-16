// Fill out your copyright notice in the Description page of Project Settings.


#include "Corelogic/Player/VPPlayer.h"

#include "Camera/CameraComponent.h"

AVPPlayer::AVPPlayer()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
}

void AVPPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AVPPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
