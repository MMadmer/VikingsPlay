// Base game components project. All rights reserved.

#include "Components/BGCStaminaComponent.h"

UBGCStaminaComponent::UBGCStaminaComponent(): bFrozen(false), bAutoRegen(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	PrimaryComponentTick.TickInterval = 1.0f / static_cast<float>(StaminaRegenFrameRate);
	StaminaModifier = MaxStamina / (StaminaRegenTime * StaminaRegenFrameRate);
}

void UBGCStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	SetStamina(MaxStamina);
}

void UBGCStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	StaminaUpdate();
}

void UBGCStaminaComponent::StaminaUpdate()
{
	SetStamina(Stamina + StaminaModifier);

	if (IsStaminaFull()) SetComponentTickEnabled(false);
}

void UBGCStaminaComponent::SetStamina(const float NewStamina)
{
	if (FMath::IsNearlyEqual(NewStamina, Stamina)) return;

	const float NextStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
	const float StaminaDelta = NextStamina - Stamina;

	Stamina = NextStamina;
	OnStaminaChanged.Broadcast(Stamina, StaminaDelta);

	if (IsEmpty()) OnStaminaEmpty.Broadcast();

	if (StaminaDelta < 0) SetComponentTickEnabled(false);

	const UWorld* World = GetWorld();
	if (World && Stamina < MaxStamina)
		World->GetTimerManager().SetTimer(StaminaTimerHandle, [&]
		{
			SetComponentTickEnabled(true);
		}, StaminaRegenDelay, false);
}
