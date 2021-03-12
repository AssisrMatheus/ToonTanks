// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

// Sets default values
APawnTurret::APawnTurret()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
  Super::BeginPlay();

  GetWorldTimerManager().SetTimer(FireRateHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

  PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (PlayerPawn && ReturnDistanceToPlayer() <= FireRange)
  {
    RotateTurret(PlayerPawn->GetActorLocation());
  }
}

void APawnTurret::CheckFireCondition()
{
  if (PlayerPawn && ReturnDistanceToPlayer() <= FireRange && PlayerPawn->GetIsPlayerAlive())
  {
    Fire();
  }
}

float APawnTurret::ReturnDistanceToPlayer()
{
  if (PlayerPawn)
  {
    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
  }

  return 0.f;
}

void APawnTurret::HandleDestruction()
{
  Super::HandleDestruction();
  Destroy();
}