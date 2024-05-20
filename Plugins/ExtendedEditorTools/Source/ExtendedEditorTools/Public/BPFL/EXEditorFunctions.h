// Sky Faller. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExtendedEditorTools/EXCoreTypes.h"
#include "EXEditorFunctions.generated.h"

/**
 * 
 */
UCLASS()
class EXTENDEDEDITORTOOLS_API UEXEditorFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Create blueprint asset based on level actor instance. Auto-rename if asset name already exists.
	 * Do nothing and print to log if directory path doesn't exist.
	 */
	UFUNCTION(BlueprintCallable, Category="Extended Editor Tools")
	static UBlueprint*
	CreateBlueprintFromActorInstance(AActor* ActorInstance, const FString& Path = "/Game/",
	                                 const FString& Name = "BP_ActorBlueprint");

	/** Send message notify to editor window and into output log. */
	UFUNCTION(BlueprintCallable, Category="Extended Editor Tools")
	static void NotifyWithLog(const FString Message, const EEXLogVerbosity LogVerbosity = Display,
	                          const float Duration = 1.0f);
};
