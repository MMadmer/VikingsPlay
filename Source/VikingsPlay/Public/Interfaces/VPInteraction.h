// Viking's play by IT-Hatters. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VPInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVPInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VIKINGSPLAY_API IVPInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(AActor* Initiator, FHitResult HitResult);
};
