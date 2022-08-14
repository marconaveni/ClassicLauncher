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
	bool valid = false;
	int32 Size = 64;
	UTexture2D* Image = UClassicFunctionLibrary::LoadTexture2DFromFile(PathTemp, valid, EClassicImageFormat::PNG, Size, Size);
	if (valid) 
	{
		OnLoadedImage.Broadcast(Image,valid);
	}
	else 
	{
		OnFailLoadedImage.Broadcast(Image, valid);
	}
}
