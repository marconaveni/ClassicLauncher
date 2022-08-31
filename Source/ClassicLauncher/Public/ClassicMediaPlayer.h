// Copyright 2022 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClassicMediaPlayer.generated.h"


UCLASS()
class CLASSICLAUNCHER_API AClassicMediaPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClassicMediaPlayer();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UMediaSoundComponent* ClassicPlayerMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UMediaSoundComponent* ClassicPlayerVideo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	FString MediaPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Media|Variables")
    TArray<FString> MediaFiles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	int32 Random;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void SetMusics(const FString NewMediaPath);

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void PlayMusic();
	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void PlayVideo(const FString File);

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void PauseMusic();
	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void PauseVideo();

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void ResumeMusic();
	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void ResumeVideo();

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void OnEndMusic();
	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void OnEndVideo();
};
