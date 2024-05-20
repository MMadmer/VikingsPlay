// Sky Faller. All rights reserved.


#include "BPFL/EXEditorFunctions.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Logging/MessageLog.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

UBlueprint* UEXEditorFunctions::CreateBlueprintFromActorInstance(AActor* ActorInstance,
                                                                 const FString& Path,
                                                                 const FString& Name)
{
	UBlueprint* NewActorBlueprint = nullptr;
#if WITH_EDITOR

	// Actor instance is valid
	if (ActorInstance)
	{
		const UWorld* World = ActorInstance->GetWorld();

		if (World)
		{
			// Package path
			FString BlueprintName = Name + TEXT("_1");
			const FString PackagePath = Path;
			FString PackageName = FString::Printf(TEXT("%s%s"), *PackagePath, *BlueprintName);

			const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
				"AssetRegistry");
			const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

			// Validate path
			if (!AssetRegistry.PathExists(PackagePath))
			{
				UE_LOG(LogTemp, Warning, TEXT("Path invalid: %s"), *PackagePath);
				return nullptr;
			}

			// Validate asset name
			int32 NameCounter = 2;
			while (AssetRegistry.GetAssetByObjectPath(FName(*(PackageName + TEXT(".") + BlueprintName)), true).
			                     IsValid())
			{
				TArray<FString> Substrings;
				BlueprintName.ParseIntoArray(Substrings, TEXT("_"));

				BlueprintName = BlueprintName.LeftChop(Substrings.Last().Len()) + FString::FromInt(NameCounter++);

				PackageName = FString::Printf(TEXT("%s%s"), *PackagePath, *BlueprintName);
				UE_LOG(LogTemp, Display, TEXT("New path: %s"), *(PackageName + TEXT(".") + BlueprintName));
			}

			// Blueprint params
			FKismetEditorUtilities::FCreateBlueprintFromActorParams BlueprintFromActorParams{};
			BlueprintFromActorParams.bReplaceActor = false;
			BlueprintFromActorParams.bOpenBlueprint = false;

			// Create new blueprint
			NewActorBlueprint = FKismetEditorUtilities::CreateBlueprintFromActor(
				PackageName, ActorInstance, BlueprintFromActorParams);

			if (NewActorBlueprint)
			{
				// Save actor blueprint
				NewActorBlueprint->MarkPackageDirty();
				FAssetRegistryModule::AssetCreated(NewActorBlueprint);
				UE_LOG(LogTemp, Display, TEXT("Actor blueprint created: %s"), *NewActorBlueprint->GetPathName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create actor blueprint: %s"), *PackageName);
			}
		}
	}

#endif
	return NewActorBlueprint;
}

void UEXEditorFunctions::NotifyWithLog(const FString Message, const EEXLogVerbosity LogVerbosity, const float Duration)
{
#if WITH_EDITOR
	// Log message
	FMessageLog MessageLog(FName(TEXT("EXEditorTools")));

	FString VerbosityText = TEXT("Display: ");
	EMessageSeverity::Type Level;
	switch (LogVerbosity)
	{
	case Display:
		Level = EMessageSeverity::Info;
		VerbosityText = TEXT("Display: ");
		break;
	case Warning:
		Level = EMessageSeverity::Warning;
		VerbosityText = TEXT("Warning: ");
		break;
	case Error:
		Level = EMessageSeverity::Error;
		VerbosityText = TEXT("Error: ");
		break;
	default:
		Level = EMessageSeverity::Info;
		VerbosityText = TEXT("Display: ");
	}
	MessageLog.Message(Level, FText::FromString(VerbosityText + Message));

	// Notify message
	FNotificationInfo Notification(FText::FromString(VerbosityText + Message));
	Notification.ExpireDuration = Duration;
	FSlateNotificationManager::Get().AddNotification(Notification);
#endif
}
