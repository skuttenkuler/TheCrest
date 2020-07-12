// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    
    //muzzle socket component
    MuzzleSocketName = "MuzzleSocket";  

}

void ASWeapon::PullTrigger()
{
    AActor* Owner =  GetOwner();
    //check for truthy pawn owner
    if(Owner)
    {
        FVector EyeLocation;
        FRotator EyeRotation;
        Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
        
        FVector ShotDirection = EyeRotation.Vector();
        FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
        
        
        
        //set params I want line trace to ignore, pawna and gun
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Owner);
        Params.AddIgnoredActor(this);
        //trace every triangle in mesh
        Params.bTraceComplex = true;
        //trace line with start point and endpoint
        FHitResult Hit;
        //if tuthy hit then do
        if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility))
        {
           //hit. do damage
            //get actor
            AActor* HitActor = Hit.GetActor();
            
            UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, Owner->GetInstigatorController(), this, DamageType);
            //if impact effect is assigned
            if(ImpactEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
            }
        }
           
        DrawDebugLine(GetWorld(),EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
        //check if muzzle effect is assigned
        if(MuzzleEffect)
        {
            UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
        }
        
    }
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

