// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    //spring arm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    //set spring arm rotation to that of pawn character and attach to root
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->SetupAttachment(RootComponent);
    //camera on character
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    //attach camera to spirng arm
    Camera->SetupAttachment(SpringArm);
   
    
    
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Value)
{
    AddMovementInput(GetActorForwardVector()* Value);
}


void ASCharacter::MoveRight(float Value)
{
    AddMovementInput(GetActorRightVector()* Value);
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    //player movement
    PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
    
    PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookRight", this, &ASCharacter::AddControllerYawInput);
    
}

