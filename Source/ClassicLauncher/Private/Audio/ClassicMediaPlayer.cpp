// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "Audio/ClassicMediaPlayer.h"

#include "RuntimeAudioImporterLibrary.h"
#include "Runtime/MediaAssets/Public/MediaSoundComponent.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "UI/Screens/MainScreen.h"
#include "Components/Image.h"
#include "Data/DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Layout/FooterDetails.h"

#define LOCTEXT_NAMESPACE "PlayerMusic"

// Sets default values
AClassicMediaPlayer::AClassicMediaPlayer()
{
	ClassicPlayerMusic = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Classic Music Media Player"));
	ClassicPlayerVideo = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Classic Video Media Player"));

	MediaPath = TEXT("file://") + UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("musics");
	MediaFiles.Empty();
	Random = -1;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClassicMediaPlayer::BeginPlay()
{
	Super::BeginPlay();

	DataManager = GetWorld()->GetSubsystem<UDataManager>();
	DataManager->SetClassicMediaPlayerReference(this);

	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		FConfig ConfigurationData;
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), ConfigurationData);
		ChangeMasterVolume(ConfigurationData.VolumeMaster);
		ChangeMusicVolume(ConfigurationData.VolumeMusic);
		ChangeVideoVolume(ConfigurationData.VolumeVideo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Not Found"), *GameRoot);
	}

	ClassicPlayerVideo->GetMediaPlayer()->OnEndReached.AddDynamic(this, &AClassicMediaPlayer::OnEndVideo);
	ClassicPlayerMusic->GetMediaPlayer()->OnEndReached.AddDynamic(this, &AClassicMediaPlayer::OnEndMusic);
}

// Called every frame
void AClassicMediaPlayer::Tick(float DeltaTime)
{
	if (ClassicPlayerVideo->GetMediaPlayer()->IsPlaying() && IsValid(DataManager->GetMainScreenReference()))
	{
		if (DoOnceIsPlayVideo.Execute())
		{
			const FIntPoint VideoDimensions = ClassicPlayerVideo->GetMediaPlayer()->GetVideoTrackDimensions(0, 0);
			FSlateBrush InBrush = DataManager->GetMainScreenReference()->FooterDetails->Video->GetBrush();
			InBrush.SetImageSize(FVector2f(VideoDimensions.X, VideoDimensions.Y));
			DataManager->GetMainScreenReference()->FooterDetails->Video->SetBrush(InBrush);
		}
	}
	else
	{
		DoOnceIsPlayVideo.Reset();
	}
	Super::Tick(DeltaTime);
}

void AClassicMediaPlayer::SetMusics(const FString NewMediaPath)
{
	MediaPath = (NewMediaPath != TEXT("")) ? TEXT("file://") + NewMediaPath : TEXT("file://") + UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("musics");
	UClassicFunctionLibrary::VerifyOrCreateDirectory(MediaPath);
	const FString Path = (NewMediaPath != TEXT("")) ? NewMediaPath + TEXT("\\") : UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("musics");;
	MediaFiles.Empty();
	UClassicFunctionLibrary::ClassicGetFiles(MediaFiles, Path, TEXT("wav"));
	UClassicFunctionLibrary::ClassicGetFiles(MediaFiles, Path, TEXT("mp3"));
}

void AClassicMediaPlayer::PlaylistMusic(const bool bShowMessage)
{
	if (MediaFiles.Num() > 0)
	{
		Random = UClassicFunctionLibrary::GenerateNumberWithoutRepeat(Random, 0, MediaFiles.Num() - 1);
		const FString File = MediaPath + TEXT("\\") + MediaFiles[Random];
		PlayMusic(File, bShowMessage);
		UE_LOG(LogTemp, Warning, TEXT("File name is %s"), *File);
	}
}

void AClassicMediaPlayer::PlayMusic(const FString File, const bool bShowMessage)
{
	if (ClassicPlayerMusic->GetMediaPlayer()->CanPlayUrl(File))
	{
		if (MusicPath.Equals(File)) return;

		if (IsValid(DataManager->GetMainScreenReference()) && bShowMessage)
		{
			const FText TextPlayerMusic = FText::FromString(FPaths::GetBaseFilename(File, true));
			FFormatNamedArguments Args;
			Args.Add("TextPlayerMusic", TextPlayerMusic);
			DataManager->GetMainScreenReference()->ShowMessage(FText::Format(LOCTEXT("Play", "Playing {TextPlayerMusic}"), Args), 3.5f);
		}
		PauseVideo();
		MusicPath = File;
		ClassicPlayerMusic->GetMediaPlayer()->OpenUrl(File);
	}
}

void AClassicMediaPlayer::PlayVideo(const FString File)
{
	const FString FileVideo = TEXT("file://") + File;
	if (ClassicPlayerVideo->GetMediaPlayer()->CanPlayUrl(FileVideo))
	{
		PauseMusic();
		ClassicPlayerVideo->GetMediaPlayer()->OpenUrl(FileVideo);
	}
}

void AClassicMediaPlayer::PauseMusic()
{
	ClassicPlayerMusic->GetMediaPlayer()->Pause();
}

void AClassicMediaPlayer::PauseVideo()
{
	ClassicPlayerVideo->GetMediaPlayer()->Pause();
}


void AClassicMediaPlayer::ResumeMusic()
{
	ClassicPlayerMusic->GetMediaPlayer()->Play();
}

void AClassicMediaPlayer::ResumeVideo()
{
	ClassicPlayerVideo->GetMediaPlayer()->Play();
}

void AClassicMediaPlayer::StopMusic()
{
	ClassicPlayerMusic->GetMediaPlayer()->Close();
}

void AClassicMediaPlayer::StopVideo()
{
	ClassicPlayerVideo->GetMediaPlayer()->Close();
}

void AClassicMediaPlayer::OnEndMusic()
{
	PlaylistMusic();
}

void AClassicMediaPlayer::OnEndVideo()
{
	if (IsValid(DataManager->GetMainScreenReference()))
	{
		DataManager->GetMainScreenReference()->FooterDetails->SetImage();
		UE_LOG(LogTemp, Warning, TEXT("Call Function MainInterface in Classic Media Player"));
	}
	ResumeMusic();
}

void AClassicMediaPlayer::ChangeMasterVolume(int32 Volume)
{
	if (MasterSoundMix != nullptr && MasterSound != nullptr)
	{
		MasterVolume = Volume;
		UGameplayStatics::SetSoundMixClassOverride(this, MasterSoundMix, MasterSound, MasterVolume * 0.01f, 1.0f, 0.0f, true);
	}
}

void AClassicMediaPlayer::ChangeMusicVolume(int32 Volume)
{
	if (MasterSoundMix != nullptr && MusicSound != nullptr)
	{
		MusicVolume = Volume;
		UGameplayStatics::SetSoundMixClassOverride(this, MasterSoundMix, MusicSound, MusicVolume * 0.01f, 1.0f, 0.0f, true);
	}
}

void AClassicMediaPlayer::ChangeVideoVolume(int32 Volume)
{
	if (MasterSoundMix != nullptr && VideoSound != nullptr)
	{
		VideoVolume = Volume;
		UGameplayStatics::SetSoundMixClassOverride(this, MasterSoundMix, VideoSound, VideoVolume * 0.01f, 1.0f, 0.0f, true);
	}
}

int32 AClassicMediaPlayer::GetMasterVolume() const
{
	return MasterVolume;
}

int32 AClassicMediaPlayer::GetMusicVolume() const
{
	return MusicVolume;
}

int32 AClassicMediaPlayer::GetVideoVolume() const
{
	return VideoVolume;
}

bool AClassicMediaPlayer::CreateRuntimeAudioImporter(URuntimeAudioImporterLibrary*& RuntimeAudioImporter)
{
	RuntimeAudioImporter = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();

	if (!IsValid(RuntimeAudioImporter))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create audio importer"));
		return false;
	}
	return true;
}

void AClassicMediaPlayer::ImportAudioClick(FString Path, FOnAudioFinalize Out)
{
	if (!CreateRuntimeAudioImporter(RuntimeAudioImporterClick)) return;

	RuntimeAudioImporterClick->OnResultNative.AddWeakLambda(this, [this, Out](URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status)
	{
		if (Status == ERuntimeImportStatus::SuccessfulImport)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully imported audio with sound wave %s"), *ImportedSoundWave->GetName());
			ClickSound = ImportedSoundWave;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to import audio"));
			ClickSound = DefaultClickSound;
		}
		if (Out.IsBound())
		{
			Out.Execute();
		}
		RuntimeAudioImporterClick = nullptr;
	});

	RuntimeAudioImporterClick->ImportAudioFromFile(Path, ERuntimeAudioFormat::Auto);
}

void AClassicMediaPlayer::ImportAudioCursor(FString Path, FOnAudioFinalize Out)
{
	if (!CreateRuntimeAudioImporter(RuntimeAudioImporterCursor)) return;

	RuntimeAudioImporterCursor->OnResultNative.AddWeakLambda(this, [this, Out](URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status)
	{
		if (Status == ERuntimeImportStatus::SuccessfulImport)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully imported audio with sound wave %s"), *ImportedSoundWave->GetName());
			CursorSound = ImportedSoundWave;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to import audio"));
			CursorSound = DefaultCursorSound;
		}
		if (Out.IsBound())
		{
			Out.Execute();
		}

		RuntimeAudioImporterCursor = nullptr;
	});

	RuntimeAudioImporterCursor->ImportAudioFromFile(Path, ERuntimeAudioFormat::Auto);
}

void AClassicMediaPlayer::GetSounds(USoundBase*& SoundCursor, USoundBase*& SoundClick) const
{
	SoundCursor = CursorSound;
	SoundClick = ClickSound;
}


#undef LOCTEXT_NAMESPACE
