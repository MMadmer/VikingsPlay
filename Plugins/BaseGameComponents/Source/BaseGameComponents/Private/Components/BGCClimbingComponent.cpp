#include "Components/BGCClimbingComponent.h"

UBGCClimbingComponent::UBGCClimbingComponent(): bWantsToClimb(false), bCanClimb(false), bClimbByTag(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval = 1.0f / static_cast<float>(CheckFrameRate);
}

void UBGCClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!World) return;

	// Bind async traces function
	CheckTraceDelegate.BindUObject(this, &ThisClass::OnCheckTraceDone);
}

void UBGCClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* TraceWorld = GetWorld();
	if (!TraceWorld) return;

	const FVector TraceStart = GetComponentLocation();
	const FVector TraceEnd = TraceStart + GetForwardVector() * ClimbDistance;

	FCollisionQueryParams CollisionParams{};
	CollisionParams.AddIgnoredActor(GetOwner());

	TraceWorld->AsyncLineTraceByChannel(EAsyncTraceType::Single, TraceStart, TraceEnd, ECC_Visibility,
	                                    CollisionParams, FCollisionResponseParams(), &CheckTraceDelegate);
}

void UBGCClimbingComponent::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);

	CheckTraceDelegate.Unbind();

	const UWorld* World = GetWorld();
	if (!World) return;

	World->GetTimerManager().ClearTimer(CheckTimerHandle);
}

void UBGCClimbingComponent::OnCheckTraceDone(const FTraceHandle& TraceHandle, FTraceDatum& TraceDatum)
{
	if (TraceDatum.OutHits.Num() <= 0)
	{
		OnCheckTraceFail();

		return;
	}

	FHitResult& HitResult = TraceDatum.OutHits[0];
	const AActor* ActorHit = HitResult.GetActor();

	if (!TraceDatum.OutHits[0].bBlockingHit || !ActorHit || bClimbByTag
		&& !ActorHit->Tags.Contains(ClimbTag))
	{
		OnCheckTraceFail();
		return;
	}

	OnCheckTraceComplete();
	OnFoundClimbingSurface.Broadcast(HitResult);
	Climbing(HitResult);
}

void UBGCClimbingComponent::OnCheckTraceFail()
{
	if (bCanClimb)
	{
		if (IsClimbing()) OnStopClimbing.Broadcast();

		bCanClimb = false;
		OnCanClimbChange.Broadcast(bCanClimb);
	}
}

void UBGCClimbingComponent::OnCheckTraceComplete()
{
	if (!bCanClimb)
	{
		if (bWantsToClimb) OnStartClimbing.Broadcast();

		bCanClimb = true;
		OnCanClimbChange.Broadcast(bCanClimb);
	}
}

void UBGCClimbingComponent::Climbing(FHitResult& SurfaceHitResult)
{
}

void UBGCClimbingComponent::SetWantsToClimb(const bool WantsToClimb)
{
	if (bWantsToClimb == WantsToClimb) return;

	if (IsClimbing()) OnStopClimbing.Broadcast();
	else
	{
		if (bCanClimb && !bWantsToClimb) OnStartClimbing.Broadcast();
	}

	bWantsToClimb = WantsToClimb;
}
