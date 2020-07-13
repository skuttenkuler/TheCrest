// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ASWeapon;

UCLASS()
class THECREST_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    void MoveForward(float Value);
    void MoveRight(float Value);
    //couch
    void BeginCrouch();
    void EndCrouch();
    
    
    
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArm;

    //zoom
    bool bWantsToZoom;
    
    UPROPERTY(EditDefaultsOnly, Category = "Player")
    float ZoomedFOV;
    
    UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin=0.1, ClampMax=100))
    float ZoompInterpSpeed;
    
    //default view
    float DefaultFOV;
    
    void BeginZoom();
    void EndZoom();
    
    ASWeapon* CurrentWeapon;
    
    UPROPERTY(EditDefaultsOnly, Category = "Player")
    TSubclassOf<ASWeapon> StarterWeaponClass;
    UPROPERTY(VisibleDefaultsOnly, Category = "Player")
    FName WeaponAttachSocketName;
    void Fire();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual FVector GetPawnViewLocation() const override;
};
