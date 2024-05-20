// Base game components project. All rights reserved.


#include "Components/BGCCharacterMovementComponent.h"

UBGCCharacterMovementComponent::UBGCCharacterMovementComponent(): bWantsToRun(false), bWasRunning(false)
{
}

void UBGCCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bWasRunning != IsRunning())
	{
		bWasRunning = IsRunning();

		if (IsRunning()) OnStartRunning.Broadcast();
		else OnStopRunning.Broadcast();
	}
}

bool UBGCCharacterMovementComponent::IsMoving() const
{
	const AActor* Owner = GetOwner();

	return Owner->GetVelocity().IsZero() && !IsFalling();
}

bool UBGCCharacterMovementComponent::IsRunning() const
{
	const AActor* Owner = GetOwner();

	return bWantsToRun && Owner && !Owner->GetVelocity().IsZero();
}

float UBGCCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();

	return IsRunning() && !IsFalling() ? MaxSpeed * RunModifier : MaxSpeed;
}
