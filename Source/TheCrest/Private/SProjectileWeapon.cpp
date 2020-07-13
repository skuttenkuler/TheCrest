// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileWeapon.h"

void ASProjectileWeapon::Fire()
{
    AActor* Owner =  GetOwner();
    //check for truthy pawn owner
    if(Owner && ProjectileClass)
    {
        FVector EyeLocation;
        FRotator EyeRotation;
        Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
        //muzzle location
        FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);
    }
}
