// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DoOnce.h"
#include "GameFramework/Actor.h"
#include "ClassicMediaPlayer.generated.h"

class URuntimeAudioImporterLibrary;

DECLARE_DYNAMIC_DELEGATE(FOnAudioFinalize);

UCLASS()
class CLASSICLAUNCHER_API AClassicMediaPlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClassicMediaPlayer();

protected:

	UPROPERTY()
	class UDataManager* DataManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	class USoundClass* MasterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	class USoundClass* MusicSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	class USoundClass* VideoSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass")
	class USoundMix* MasterSoundMix;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	int32 MasterVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	int32 MusicVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	int32 VideoVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	FDoOnce DoOnceIsPlayVideo;

	UPROPERTY()
	FString MusicPath;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void SetMusics(const FString NewMediaPath);

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void PlaylistMusic(const bool bShowMessage = true);
	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void PlayMusic(const FString File, const bool bShowMessage = true);
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
	void StopMusic();
	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void StopVideo();

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void OnEndMusic();
	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void OnEndVideo();

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void ChangeMasterVolume(int32 Volume);

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void ChangeMusicVolume(int32 Volume);

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void ChangeVideoVolume(int32 Volume);

	UFUNCTION(BlueprintPure, Category = "Media|Functions")
	int32 GetMasterVolume() const;

	UFUNCTION(BlueprintPure, Category = "Media|Functions")
	int32 GetMusicVolume() const;

	UFUNCTION(BlueprintPure, Category = "Media|Functions")
	int32 GetVideoVolume() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	USoundBase* DefaultCursorSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media|Variables")
	USoundBase* DefaultClickSound;

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void ImportAudioClick(FString Path, FOnAudioFinalize Out);

	UFUNCTION(BlueprintCallable, Category = "Media|Functions")
	void ImportAudioCursor(FString Path, FOnAudioFinalize Out);

	UFUNCTION(BlueprintPure, Category = "Media|Functions")
	void GetSounds(USoundBase*& SoundCursor, USoundBase*& SoundClick) const;

private:
	UFUNCTION()
	bool CreateRuntimeAudioImporter(URuntimeAudioImporterLibrary*& RuntimeAudioImporter);

	UPROPERTY()
	URuntimeAudioImporterLibrary* RuntimeAudioImporterClick;

	UPROPERTY()
	URuntimeAudioImporterLibrary* RuntimeAudioImporterCursor;

	UPROPERTY()
	USoundBase* CursorSound;

	UPROPERTY()
	USoundBase* ClickSound;
};
