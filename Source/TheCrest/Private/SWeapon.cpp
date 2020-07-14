// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "../TheCrest.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
                TEXT("COOP.DebugWeapons"),
                DebugWeaponDrawing,
                TEXT("Draw Debug Lines for Weapons"),
                ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
    
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    
    //muzzle socket component
    MuzzleSocketName = "MuzzleSocket";
    //tracer target
    TracerTargetName = "Target";
    //damage
    BaseDamage = 20.0f;

}

void ASWeapon::Fire()
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
        Params.bReturnPhysicalMaterial = true;
        //particle target param
        FVector TracerEndPoint = TraceEnd;
        //trace line with start point and endpoint
        FHitResult Hit;
        //if tuthy hit then do
        if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, Params))
        {
           //hit. do damage
            //get actor
            AActor* HitActor = Hit.GetActor();
            
            //get surface type
            EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
            float ActualDamage = BaseDamage;
            if(SurfaceType == SURFACE_FLESHVULNERABLE)
            {
                ActualDamage *= 4.0f;
            }
            UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, Owner->GetInstigatorController(), this, DamageType);
           
            UParticleSystem* SelectedEffect = nullptr;
            //switch case for different hit impacts
            switch (SurfaceType)
            {
                case SURFACE_FLESHDEFAULT:
                case SURFACE_FLESHVULNERABLE:
                    SelectedEffect = FleshImpactEffect;
                    break;
                default:
                    SelectedEffect = DefaultImpactEffect;
                    break;
            }
            //if impact effect is assigned
            if(SelectedEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
            }

            //if we don't hit a break point or character, use endpoint of tracer
            TracerEndPoint = Hit.ImpactPoint;
        }
           
        if(DebugWeaponDrawing > 0)
        {
            DrawDebugLine(GetWorld(),EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
        }
        PlayFireEffects(TracerEndPoint);
        
    }
}
void ASWeapon::PlayFireEffects(FVector TraceEnd)
{
    //check if muzzle effect is assigned
     if(MuzzleEffect)
     {
         UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
     }
    
     if(TracerEffect)
     {
         //get location for muzzle
         FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
         UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
         if(TracerComponent)
         {
             TracerComponent->SetVectorParameter(TracerTargetName, TraceEnd);
         }
     }
    
    //camera shake
    //point to controller
   APawn* MyOwner = Cast<APawn>(GetOwner());
    if (MyOwner)
    {
        APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
        if (PC)
        {
            PC->ClientPlayCameraShake(FireCameraShake);
        }
    }
}
    
