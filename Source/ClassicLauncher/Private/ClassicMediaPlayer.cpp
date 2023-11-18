// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "ClassicMediaPlayer.h"
#include "Runtime/MediaAssets/Public/MediaSoundComponent.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "ClassicFunctionLibrary.h"
#include "MainInterface.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

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

	FString ConfigResult;
	const FString GameRoot = UClassicFunctionLibrary::GetGameRootDirectory() + TEXT("config\\config.xml");

	if (UClassicFunctionLibrary::LoadStringFile(ConfigResult, GameRoot))
	{
		FConfig ConfigurationData;
		UClassicFunctionLibrary::SetConfig(UClassicFunctionLibrary::LoadXMLSingle(ConfigResult, TEXT("config")), ConfigurationData);
		ChangeMasterVolume(ConfigurationData.Volume);
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

	if (ClassicPlayerVideo->GetMediaPlayer()->IsPlaying() && MainInterfaceReference != nullptr)
	{
		if (DoOnceIsPlayVideo.Execute())
		{
			const FIntPoint VideoDimensions = ClassicPlayerVideo->GetMediaPlayer()->GetVideoTrackDimensions(0, 0);
			FSlateBrush InBrush = MainInterfaceReference->ImgVideo->GetBrush();
			InBrush.SetImageSize(FVector2f(VideoDimensions.X, VideoDimensions.Y));
			MainInterfaceReference->ImgVideo->SetBrush(InBrush);
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
		if (MainInterfaceReference != nullptr && bShowMessage)
		{
			const FText TextPlayerMusic = FText::FromString(FPaths::GetBaseFilename(File, true));
			FFormatNamedArguments Args;
			Args.Add("TextPlayerMusic", TextPlayerMusic);
			MainInterfaceReference->ShowMessage(FText::Format(LOCTEXT("Play", "Playing {TextPlayerMusic}"), Args), 3.5f);
		}
		PauseVideo();
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
	if (MainInterfaceReference != nullptr)
	{
		MainInterfaceReference->SetImageBottom();
		UE_LOG(LogTemp, Warning, TEXT("Call Function MainInterface in Classic Media Player"));
	}
	ResumeMusic();
}

void AClassicMediaPlayer::ChangeMasterVolume(int32 Volume)
{
	if (MasterSoundMix != nullptr && MasterSound != nullptr)
	{
		MasterVolume = Volume;
		const float NewVolume = FMath::Clamp(Volume * 0.01f, 0.01f, 1.0f);
		UGameplayStatics::SetSoundMixClassOverride(this, MasterSoundMix, MasterSound, Volume * 0.01f, 1.0f, 0.0f, true);
	}
}

int32 AClassicMediaPlayer::GetMasterVolume()
{
	return MasterVolume;
}

#undef LOCTEXT_NAMESPACE