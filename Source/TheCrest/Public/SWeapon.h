// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class DamageType;
class UParticleSystem;

UCLASS()
class THECREST_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
    
    virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComponent;
    
    void PlayFireEffects(FVector TraceEnd);
    
    void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UDamageType> DamageType;
    //muzzle socket
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName;
    //tracer target
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName TracerTargetName;
    //muzzle effect
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* MuzzleEffect;
    //impact effect
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* DefaultImpactEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* FleshImpactEffect;
    //tracer effect
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* TracerEffect;
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<UCameraShake> FireCameraShake;
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float BaseDamage;
    
    void Fire();
    
    FTimerHandle TimerHandle_TimeBetweenShots;
    
    float LastFireTime;
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float RateOfFire;
    
    float TimeBetweenShots;
    
public:
  
    
    void StartFire();
    
    void StopFire();
};
