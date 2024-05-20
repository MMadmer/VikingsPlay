// Base game components project. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BGCHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, HealthDelta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBarriersChangedSignature, uint8, NewBarriers, uint8, BarriersDelta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASEGAMECOMPONENTS_API UBGCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBGCHealthComponent();

	/** Calls when component owner is dead(health = 0).*/
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeathSignature OnDeath;

	/** Calls only when health changed.*/
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChangedSignature OnHealthChanged;

	/** Calls only when barriers changed.*/
	UPROPERTY(BlueprintAssignable, Category="Protection")
	FOnBarriersChangedSignature OnBarriersChanged;

	/** If true, health will "freeze"(damage will not apply, not working if damage is pure).*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	uint8 bImmortal : 1;

	/** Maximum health.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0"))
	float MaxHealth = 100.0f;

	/** Maximum Barriers.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Protection")
	uint8 MaxBarriers = 0;

	/** Check on death.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	/** Get current health.*/
	UFUNCTION(BlueprintPure)
	float GetHealth() const { return Health; }

	/** Get normalized current health.*/
	UFUNCTION(BlueprintPure)
	float GetHealthNorm() const
	{
		return FMath::GetMappedRangeValueClamped(FVector2D(0.0f, MaxHealth), FVector2D(0.0f, 1.0f), Health);
	}

	/** Get current barriers.*/
	UFUNCTION(BlueprintPure, Category = "Protection")
	uint8 GetBarriers() const { return Barriers; }

	UFUNCTION(BlueprintCallable)
	void SetDamageResistance(const float NewResistance)
	{
		DamageResistance = FMath::Clamp(NewResistance, 0.0f, 1.0f);
	}

	UFUNCTION(BlueprintCallable)
	float GetResistance() const { return DamageResistance; }

	UFUNCTION(BlueprintCallable)
	void SetHealReduction(const float NewReduction)
	{
		HealReduction = FMath::Clamp(NewReduction, 0.0f, 1.0f);
	}

	UFUNCTION(BlueprintCallable)
	float GetHealReduction() const { return HealReduction; }

	/**
	 * Tries to increase health value. Not working if owner is dead.
	 * Accounts heal reduction. Ignore reduction if pure heal.*/
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay=1))
	void Heal(const float HealValue = 0.0f, const bool PureHeal = false);

protected:
	virtual void BeginPlay() override;

	/** Normalized resistance value. Decrease incoming non-pure damage.*/
	UPROPERTY(EditAnywhere, meta = (ClampMin="0", ClampMax="1", UIMin="0", UIMax="1"))
	float DamageResistance = 0.0f;

	/** Normalized reduction value. Decrease incoming non-pure heal.*/
	UPROPERTY(EditAnywhere, meta = (ClampMin="0", ClampMax="1", UIMin="0", UIMax="1"))
	float HealReduction = 0.0f;

	float Health = 0.0f;
	uint8 Barriers = 0;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                     AController* InstigatedBy, AActor* DamageCauser);

	void SetHealth(const float NewHealth);
	void SetBarriers(const uint8 NewBarriers);
};
