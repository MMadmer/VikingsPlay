// Fill out your copyright notice in the Description page of Project Settings.


#include "Corelogic/Subsystems/VPAudioSubsystem.h"
#include "Components/AudioComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogAudioSys, All, All);


void UVPAudioSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (AudioComponent) AudioComponent->OnAudioFinished.RemoveDynamic(this, &UVPAudioSubsystem::PlayRandomSound);
}

void UVPAudioSubsystem::PlayLevelAmbient(UAudioComponent* WorldAmbient,
                                         const TSet<TSoftObjectPtr<USoundBase>> CurrentLevelAmbient,
                                         const float VolumeNorm)
{
	AudioComponent = WorldAmbient;
	if (!AudioComponent) return;

	// Volume settings
	BaseAudioVolume = AudioComponent->VolumeMultiplier;
	SetAmbientVolume(VolumeNorm);

	AudioComponent->OnAudioFinished.AddUniqueDynamic(this, &UVPAudioSubsystem::PlayRandomSound);

	if (LevelAmbient.Num() > 0)
	{
		LevelAmbient.Empty();
		Indexes.Init(false, LevelAmbient.Num());
	}

	LevelAmbient = CurrentLevelAmbient;

	PlayRandomSound();
}

void UVPAudioSubsystem::SetAmbientVolume(const float VolumeNorm) const
{
	if (!AudioComponent) return;

	AudioComponent->SetVolumeMultiplier(FMath::Clamp(BaseAudioVolume * VolumeNorm, 0.001f, 1.0f));
}

void UVPAudioSubsystem::PlayRandomSound()
{
	if (!AudioComponent) return;

	int32 CurrentIndex;

	if (Indexes.Num() != LevelAmbient.Num() || Indexes.CountSetBits() == Indexes.Num())
		Indexes.Init(false, LevelAmbient.Num());

	if (LevelAmbient.Num() == 0)
	{
		UE_LOG(LogAudioSys, Warning, TEXT("Ambient not found"));
		return;
	}

	do
	{
		CurrentIndex = FMath::RandRange(0, LevelAmbient.Num() - 1);
	}
	while (Indexes[CurrentIndex]);
	Indexes[CurrentIndex] = true;

	auto SoundIt = LevelAmbient.CreateIterator();
	for (int32 i = 0; i < CurrentIndex; ++i)
	{
		++SoundIt;
	}

	if (SoundIt->IsNull()) return;

	// Check and use if already loaded
	USoundBase* Sound = SoundIt->Get();
	if (Sound)
	{
		AudioComponent->SetSound(Sound);
		AudioComponent->Play();
	}
	else
	{
		// Async load
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(SoundIt->ToSoftObjectPath(), [&, SoundIt]
		{
			if (!SoundIt) return;

			USoundBase* LoadedSound = Cast<USoundBase>(SoundIt->Get());
			if (LoadedSound)
			{
				if (AudioComponent)
				{
					AudioComponent->SetSound(LoadedSound);
					AudioComponent->Play();
				}
			}
			else
			{
				UE_LOG(LogAudioSys, Warning, TEXT("Failed to async load sound."));
			}
		});
	}
}
