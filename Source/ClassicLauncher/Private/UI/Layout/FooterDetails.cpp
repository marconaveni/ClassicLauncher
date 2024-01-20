// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Layout/FooterDetails.h"

#include "UI/Animations/AnimationUILoader.h"
#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "Audio/ClassicMediaPlayer.h"
#include "UI/Screens/MainScreen.h"
#include "UI/Components/TextBoxScroll.h"
#include "Components/Image.h"

void UFooterDetails::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Clear();
}

void UFooterDetails::Clear()
{
	IndexFooter = -1;
	TextBoxScroll->SetText(TEXT(""));
}

void UFooterDetails::OpenFooter(const FGameData Data, const int32 Index, EPositionY& PositionY, AClassicMediaPlayer* ClassicMediaPlayerReference)
{
	PositionY = EPositionY::BOTTOM;
	GameData = Data;
	IndexCard = Index;
	GetWorld()->GetTimerManager().ClearTimer(StartVideoTimerHandle);
	StartVideoTimerHandle.Invalidate();
	SetImage();
	TextBoxScroll->StartScroll();

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UFooterDetails::StartVideo, PositionY, ClassicMediaPlayerReference);
	GetWorld()->GetTimerManager().SetTimer(StartVideoTimerHandle, TimerDelegate, 5, false);

	UE_LOG(LogTemp, Warning, TEXT("Open Footer Details"));
}

void UFooterDetails::CloseFooter(EPositionY& PositionY, AClassicMediaPlayer* ClassicMediaPlayerReference)
{
	if (BackgroundVideo->GetRenderOpacity() == 0)
	{
		PositionY = EPositionY::CENTER;
		GetWorld()->GetTimerManager().ClearTimer(StartVideoTimerHandle);
		ClassicMediaPlayerReference->StopVideo();
		ClassicMediaPlayerReference->ResumeMusic();
		TextBoxScroll->CancelAutoScroll();
		UAnimationUILoader* AnimationLoaderUISubSystem = GetWorld()->GetSubsystem<UAnimationUILoader>();
		UAnimationUI* Animation = AnimationLoaderUISubSystem->GetAnimation("ImageToVideo");
		Animation->ClearAnimation();
		UE_LOG(LogTemp, Warning, TEXT("Close Footer Details"));
	}
	else
	{
		SetPlayAnimation(TEXT("RetainVideo"));
	}
}

void UFooterDetails::ExpandVideo()
{
	if (BackgroundVideo->GetRenderOpacity() == 0)
	{
		SetPlayAnimation(TEXT("ExpandVideo"));
	}
}

void UFooterDetails::SetImage()
{
	Image->SetRenderOpacity(1);
	Video->SetRenderOpacity(0);

	if (IndexCard == IndexFooter) return;

	FString ImagePath = TEXT("");

	if (FPaths::FileExists(GameData.thumbnailFormated))
	{
		ImagePath = GameData.thumbnailFormated;
	}
	else if (FPaths::FileExists(GameData.imageFormated))
	{
		ImagePath = GameData.imageFormated;
	}

	if (ImagePath != TEXT(""))
	{
		const EClassicImageFormat Format = UClassicFunctionLibrary::GetFormatImage(ImagePath);
		FLoadImageDelegate ImageLoadedDelegate;
		ImageLoadedDelegate.BindDynamic(this, &UFooterDetails::ImageOutThumb);
		UClassicFunctionLibrary::AsyncLoadTexture2DFromFile(ImageLoadedDelegate, ImagePath, IndexCard, Format, EClassicTextureFilter::DEFAULT);
	}
	else
	{
		ImageOutThumb(nullptr, IndexCard, false);
	}
}

void UFooterDetails::StartVideo(EPositionY PositionY, AClassicMediaPlayer* ClassicMediaPlayerReference)
{
	if (PositionY != EPositionY::BOTTOM) return;

	const FString PathVideo = GameData.videoFormated;
	if (FPaths::FileExists(PathVideo))
	{
		SetPlayAnimation(TEXT("ImageToVideo"));
		ClassicMediaPlayerReference->PlayVideo(PathVideo);
	}
}

void UFooterDetails::ImageOutThumb(UTexture2D* TextureOut, int32 Index, bool Sucesseful)
{
	if (TextureOut == nullptr || !Sucesseful)
	{
		TextureOut = ImageDefault;
	}
	FSlateBrush NewBrush;
	NewBrush.SetImageSize(FVector2D(TextureOut->GetSizeX() * 2, TextureOut->GetSizeY() * 2));
	NewBrush.SetResourceObject(TextureOut);
	Image->SetBrush(NewBrush);
	IndexFooter = Index;
}

void UFooterDetails::SetTheme(UTexture2D* TextureBackground)
{
	Background->SetBrushFromTexture(TextureBackground);
}
