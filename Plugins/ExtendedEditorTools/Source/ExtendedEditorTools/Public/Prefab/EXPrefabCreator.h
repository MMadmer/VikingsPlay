// Sky Faller. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EXPrefabCreator.generated.h"

class AUTPrefab;

UCLASS()
class EXTENDEDEDITORTOOLS_API AEXPrefabCreator : public AActor
{
	GENERATED_BODY()

public:
	AEXPrefabCreator();

protected:
	/** Template of generated platforms class.*/
	UPROPERTY(EditAnywhere, Category="Editor Tools")
	TSubclassOf<AUTPrefab> ParentClass;

	UPROPERTY(EditAnywhere, Category="Editor Tools")
	FString NewPrefabBaseName = TEXT("Prefab");

	/** Path to new prefab asset. Already contains /Game/*/
	UPROPERTY(EditAnywhere, Category="Editor Tools")
	FString NewPrefabPath = TEXT("");

	/** Working radius.*/
	UPROPERTY(EditAnywhere, Category="Editor Tools")
	USphereComponent* Handler;

	/** Convert all actors to platform's static mesh sockets(set PlatformTag to target platform static mesh actor).*/
	UFUNCTION(CallInEditor, Category="Editor Tools")
	void CreatePrefab();

	/** Destroy all actors apart from prefab.*/
	UFUNCTION(CallInEditor, Category="Editor Tools")
	void ClearPrefab();

	virtual void BeginPlay() override;

	AUTPrefab* FindAndRemovePrefab(TArray<AActor*>& Actors) const;
};
