// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CapsuleComponent.h"
#include "../TheCrest.h"
#include "SWeapon.h"
#include "Public/Components/SHealthComponent.h"

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
    
    //set permission to crouch
    GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
    //make sure mesh is blocking weapon trace, not the capsule component
    GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
    //CanEverCrouch() const { return NavAgentProps.bCanCrouch;
    HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("Health"));
    
    //level of zoom
    ZoomedFOV = 65.0f;
    ZoompInterpSpeed = 20;
    
    WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
    //get default view
    DefaultFOV = Camera->FieldOfView;
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //spawn default weapon
    CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    if(CurrentWeapon)
    {
        CurrentWeapon->SetOwner(this);
        CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,WeaponAttachSocketName);
    }
    
    HealthComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float Value)
{
    AddMovementInput(GetActorForwardVector()* Value);
}


void ASCharacter::MoveRight(float Value)
{
    AddMovementInput(GetActorRightVector()* Value);
}

void ASCharacter::BeginCrouch()
{
    Crouch();
    //UE_LOG(LogTemp,Warning, TEXT("Crouch pressed"));
}
void ASCharacter::EndCrouch()
{
    UnCrouch();
}

void ASCharacter::BeginZoom()
{
    bWantsToZoom = true;
}
void ASCharacter::EndZoom()
{
     bWantsToZoom = false;
}
void ASCharacter::StartFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StartFire();
    }
}


void ASCharacter::StopFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

void ASCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
   class AController* InstigatedBy, AActor* DamageCauser)
{
   if (Health <= 0.0f && !bDied)
   {
       // Die!
       bDied = true;
       //stop all movement
       GetMovementComponent()->StopMovementImmediately();
       //stop an collision with capsule
       GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

       DetachFromControllerPendingDestroy();
       
       SetLifeSpan(10.0f);

      
   }
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
    //zoom time
    float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoompInterpSpeed);
    
    Camera->SetFieldOfView(NewFOV);

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
    
    //player crouch
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);
    //player jump
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    //player FOV ZOOM
    PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
    PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);
    
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);
    
    
}

FVector ASCharacter::GetPawnViewLocation() const
{
    if(Camera)
    {
        return Camera->GetComponentLocation();
    }
    return Super::GetPawnViewLocation();
}

