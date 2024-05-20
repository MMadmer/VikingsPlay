// Sky Faller. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EXGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class EXTENDEDEDITORTOOLS_API UEXGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CustomThunk, DisplayName="RegisterRandom", meta=(ArrayParm = "TargetArray"),
		Category="Random")
	static void Array_RegisterRandom(const TArray<int32>& TargetArray);

	/*UFUNCTION(BlueprintCallable, CustomThunk,
		meta=(BlueprintInternalUseOnly = "true", DisplayName = "GetRandom", ArrayParm = "TargetArray",
			ArrayTypeDependentParams = "OutItem"))
	static void GetRandomItem(const TArray<int32>& TargetArray, int32& OutItem, int32& OutIndex);*/

	static void GenericArray_RegisterRandom(const UObject* WorldContext, void* TargetArray,
	                                        const FArrayProperty* ArrayProp);

	DECLARE_FUNCTION(execArray_RegisterRandom)
	{
		P_GET_OBJECT(UObject, OwnerObject);

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(nullptr);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;
		P_NATIVE_BEGIN;
			GenericArray_RegisterRandom(OwnerObject, ArrayAddr, ArrayProperty);
		P_NATIVE_END;
	}

private:
	TMap<void*, TBitArray<>> RandomPairs;
};
