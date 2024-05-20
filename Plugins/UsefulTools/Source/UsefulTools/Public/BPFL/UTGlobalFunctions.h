// Sky Faller. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UTGlobalFunctions.generated.h"

/**
 * 
 */
UCLASS()
class USEFULTOOLS_API UUTGlobalFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Useful Tools")
	static void DestroyAttachedActorsByClass(const AActor* OwnerActor, const TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintCallable, Category="Useful Tools")
	static void DestroyComponentsByClass(const AActor* OwnerActor, const TSubclassOf<UActorComponent> ComponentClass);
};
