// Fill out your copyright notice in the Description page of Project Settings.


#include "Corelogic/VPGameModeBase.h"

#include "Corelogic/Player/VPPlayer.h"
#include "Corelogic/Player/VPPlayerController.h"
#include "Corelogic/Player/VPPlayerState.h"


AVPGameModeBase::AVPGameModeBase()
{
	PlayerControllerClass = AVPPlayerController::StaticClass();
	DefaultPawnClass = AVPPlayer::StaticClass();
	PlayerStateClass = AVPPlayerState::StaticClass();
}
