// Sky Faller. All rights reserved.


#include "BPFL/UTGlobalFunctions.h"

void UUTGlobalFunctions::DestroyAttachedActorsByClass(const AActor* OwnerActor, const TSubclassOf<AActor> ActorClass)
{
	if (!OwnerActor) return;

	TArray<AActor*> AttachedActors;
	OwnerActor->GetAttachedActors(AttachedActors);

	for (const auto& AttachedActor : AttachedActors)
	{
		if (AttachedActor->GetClass()->IsChildOf(ActorClass)) AttachedActor->Destroy();
	}
}

void UUTGlobalFunctions::DestroyComponentsByClass(const AActor* OwnerActor,
                                                  const TSubclassOf<UActorComponent> ComponentClass)
{
	if (!OwnerActor) return;

	TArray<UActorComponent*> Components;
	OwnerActor->GetComponents<UActorComponent>(Components);

	for (const auto& Component : Components)
	{
		if (Component->GetClass()->IsChildOf(ComponentClass)) Component->DestroyComponent();
	}
}
