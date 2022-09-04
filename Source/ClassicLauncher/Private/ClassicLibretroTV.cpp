// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicLibretroTV.h"

// Sets default values
AClassicLibretroTV::AClassicLibretroTV()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClassicLibretroTV::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AClassicLibretroTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AClassicLibretroTV::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AClassicLibretroTV::OnNativeLoadRom(FString CorePath, FString RomPath, bool CanUnzip)
{
	OnLoadRom(CorePath, RomPath, CanUnzip);
}

