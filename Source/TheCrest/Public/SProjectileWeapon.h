// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THECREST_API ASProjectileWeapon : public ASWeapon
{
	GENERATED_BODY()
    
protected:

    virtual void Fire();
    
    UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
    TSubclassOf<AActor> ProjectileClass;
    
};
