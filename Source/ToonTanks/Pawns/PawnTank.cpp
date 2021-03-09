// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APawnTank::APawnTank()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
  SpringArm->SetupAttachment(RootComponent);

  Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
  Super::BeginPlay();
  PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  Rotate();
  Move();

  if (PlayerControllerRef)
  {
    FHitResult TraceHitResult;
    PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);

    FVector HitLocation = TraceHitResult.ImpactPoint;

    RotateTurret(HitLocation);
  }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
  InputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
  InputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
  InputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value)
{
  MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
  float RotateAmount = Value * TurnSpeed * GetWorld()->DeltaTimeSeconds;
  FRotator Rotator = FRotator(0, RotateAmount, 0);
  RotationDirection = FQuat(Rotator);
}

void APawnTank::Move()
{
  AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
  AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::HandleDestruction()
{
  Super::HandleDestruction();
  // Hide player
}