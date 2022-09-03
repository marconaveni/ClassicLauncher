// Copyright 2022 Marco Naveni. All Rights Reserved.

#include "ImageLoader.h"
#include "ClassicFunctionLibrary.h"


UImageLoader* UImageLoader::AsyncImageLoader(FString PathImage)
{

	UImageLoader* NewImage = NewObject<UImageLoader>();
	NewImage->PathTemp = PathImage;

	return NewImage;
}

void UImageLoader::Activate()
{
	LoadTexture();
}

void UImageLoader::LoadTexture()
{

	UTexture2D* Image = UClassicFunctionLibrary::LoadTexture(PathTemp);
	if (Image != nullptr)
	{
		OnLoadedImage.Broadcast(Image,true);
	}
	else 
	{
		OnFailLoadedImage.Broadcast(Image, false);
	}
}
