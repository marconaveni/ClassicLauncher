// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ClassicLibretroTV.generated.h"

UCLASS()
class CLASSICLAUNCHER_API AClassicLibretroTV : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AClassicLibretroTV();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "ClassicLibretroTV|Events")
	void OnNativeLoadRom(FString CorePath, FString RomPath, bool CanUnzip);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadRom(const FString& CorePath, const FString& RomPath, bool CanUnzip);

};
