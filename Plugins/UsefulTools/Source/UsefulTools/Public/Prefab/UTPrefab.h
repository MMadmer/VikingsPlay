// Sky Faller. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UniqueMesh.h"
#include "GameFramework/Actor.h"
#include "UTPrefab.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrefabLoaded);

USTRUCT(BlueprintType)
struct FPrefabInfo
{
	GENERATED_BODY()

	/** Class that will be spawned and attached to target scene component. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prefab Info")
	TSoftClassPtr<AActor> SpawnClass;

	/** Mesh to replace if spawn class is static mesh actor only. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prefab Info")
	FUniqueMesh ClassMesh;

	/** Mesh to replace if spawn class is static mesh actor only. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prefab Info")
	FName CollisionPreset;

	FPrefabInfo()
	{
		SpawnClass = nullptr;
		ClassMesh = {};
		CollisionPreset = TEXT("");
	}

	explicit FPrefabInfo(const TSoftClassPtr<AActor>& NewSpawnClass) : FPrefabInfo()
	{
		SpawnClass = NewSpawnClass;
	}

	FPrefabInfo(const TSoftClassPtr<AActor>& NewSpawnClass,
	            const FUniqueMesh& NewClassMesh,
	            const FName& NewCollisionPreset) : FPrefabInfo(NewSpawnClass)
	{
		ClassMesh = NewClassMesh;
		CollisionPreset = NewCollisionPreset;
	}
};

UCLASS()
class USEFULTOOLS_API AUTPrefab : public AActor
{
	GENERATED_BODY()

public:
	AUTPrefab();

	/** Calls when all prefab's objects finished spawning.*/
	UPROPERTY(BlueprintAssignable)
	FOnPrefabLoaded OnPrefabLoaded;

	/** Base static mesh. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn Info")
	UStaticMeshComponent* StaticMesh;

	/** Objects to spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn Info")
	TMap<FName, FPrefabInfo> SpawnObjects;

	UFUNCTION(BlueprintPure)
	bool IsPrefabLoaded() const { return SpawnedObjectsCount == SpawnObjects.Num(); };

protected:
	UPROPERTY(EditAnywhere, Category="Spawn Info")
	uint8 MinMeshesToHism = 5;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Spawn all objects from SpawnObjects map. */
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Spawn Info", meta=(Keywords="Spawn All Objects"))
	void SpawnAllObjects(const bool UseAsync = false);

	/** Destroy attached actors(not recursive) and HISM components. */
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Spawn Info", meta=(Keywords="Clear Spawned Objects"))
	void ClearAttachedObjects() const;

	/** Convert all attached static mesh actors to HISM components. */
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Spawn Info", meta=(Keywords="Convert Static Mesh To HISM"))
	void ConvertMeshToHism();

	uint32 SpawnedObjectsCount = 0;

	void ClearAllHism() const;

private:
	void SpawnLoadedObject(FPrefabInfo* const& Info, USceneComponent* const& SceneComponent);
	void SpawnLoadedMesh(FPrefabInfo* const& Info, UStaticMeshComponent* const& MeshComp, AActor* SpawnedActor);
};
