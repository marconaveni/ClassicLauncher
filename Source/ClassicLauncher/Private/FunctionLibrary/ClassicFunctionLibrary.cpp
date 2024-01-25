// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "FunctionLibrary/ClassicFunctionLibrary.h"
#include "HAL/RunnableThread.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/DateTime.h"
#include "DynamicRHI.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/Runnable.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <winreg.h>
#include "UObject/SavePackage.h"
#include "Interfaces/MusicInterface.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"

EButtonsGame UClassicFunctionLibrary::GetInputButtonsGame(const FKey& InKey)
{
	const FString KeyName = InKey.ToString();

	if (KeyName == "Left" || KeyName == "Gamepad_DPad_Left" || KeyName == "Gamepad_LeftStick_Left") return EButtonsGame::LEFT;
	if (KeyName == "Right" || KeyName == "Gamepad_DPad_Right" || KeyName == "Gamepad_LeftStick_Right") return EButtonsGame::RIGHT;
	if (KeyName == "Up" || KeyName == "Gamepad_DPad_Up" || KeyName == "Gamepad_LeftStick_Up") return EButtonsGame::UP;
	if (KeyName == "Down" || KeyName == "Gamepad_DPad_Down" || KeyName == "Gamepad_LeftStick_Down") return EButtonsGame::DOWN;
	if (KeyName == "Enter" || KeyName == "Gamepad_FaceButton_Bottom") return EButtonsGame::A;
	if (KeyName == "Backspace" || KeyName == "Escape" || KeyName == "Gamepad_FaceButton_Right") return EButtonsGame::B;
	if (KeyName == "F" || KeyName == "Gamepad_FaceButton_Top") return EButtonsGame::Y;
	if (KeyName == "S" || KeyName == "Gamepad_FaceButton_Left") return EButtonsGame::X;
	if (KeyName == "Q" || KeyName == "Gamepad_LeftShoulder") return EButtonsGame::LB;
	if (KeyName == "E" || KeyName == "Gamepad_RightShoulder") return EButtonsGame::RB;
	if (KeyName == "A" || KeyName == "Gamepad_LeftTrigger") return EButtonsGame::LT;
	if (KeyName == "D" || KeyName == "Gamepad_RightTrigger") return EButtonsGame::RT;
	if (KeyName == "LeftControl" || KeyName == "Gamepad_Special_Right") return EButtonsGame::START;
	if (KeyName == "Alt" || KeyName == "Gamepad_Special_Left") return EButtonsGame::SELECT;
	if (KeyName == "Mouse_Scroll_Up" || KeyName == "Gamepad_RightStick_Up") return EButtonsGame::SCROLLUP;
	if (KeyName == "Mouse_Scroll_Down" || KeyName == "Gamepad_RightStick_Down") return EButtonsGame::SCROLLDOWN;
	if (KeyName == "M" || KeyName == "Gamepad_RightThumbstick") return EButtonsGame::M;

	return EButtonsGame::NONE;
}

EButtonsGame UClassicFunctionLibrary::GetInputButton(const FKeyEvent& InKeyEvent)
{
	return GetInputButtonsGame(InKeyEvent.GetKey());
}

FString UClassicFunctionLibrary::GetGameRootDirectory()
{
	FString RootGameDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	RootGameDir = RootGameDir.Replace(TEXT("/"), TEXT("\\"));
	return RootGameDir;
}

void UClassicFunctionLibrary::PauseProcess(float timer)
{
	FPlatformProcess::Sleep(timer);
}

bool UClassicFunctionLibrary::SaveStringToFile(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverWriting, bool AllowAppend)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveDirectory))
	{
		//Could not make the specified directory
		return false;
		//~~~~~~~~~~~~~~~~~~~~~~
	}

	//get complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	//No over-writing?
	if (!AllowOverWriting)
	{
		//Check if file exists already
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			//no overwriting
			return false;
		}
	}

	if (AllowAppend)
	{
		SaveText += "\n";
		return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}
	return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory, FFileHelper::EEncodingOptions::ForceUTF8);
}

bool UClassicFunctionLibrary::VerifyOrCreateDirectory(const FString& TestDir)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Directory Exists?
	if (!PlatformFile.DirectoryExists(*TestDir))
	{
		PlatformFile.CreateDirectory(*TestDir);

		if (!PlatformFile.DirectoryExists(*TestDir))
		{
			return false;
		}
	}
	return true;
}

bool UClassicFunctionLibrary::VerifyDirectory(const FString& TestDir)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.DirectoryExists(*TestDir);
}

int32 UClassicFunctionLibrary::GenerateNumberWithoutRepeat(int32 Value, int32 Min, int32 Max)
{
	int32 NewValue = Value;
	if (Max > Min)
	{
		while (true)
		{
			NewValue = FMath::RandRange(Min, Max);
			if (Value != NewValue)
			{
				return NewValue;
			}
		}
	}
	return FMath::Clamp(NewValue, 0, Max);
}

FString UClassicFunctionLibrary::HomeDirectoryReplace(FString Directory)
{
	FString RootDirectory = GetGameRootDirectory();
	return Directory.Replace(TEXT("$(home)"), *RootDirectory, ESearchCase::IgnoreCase);
}

bool UClassicFunctionLibrary::ClassicGetFiles(TArray<FString>& Files, FString RootFolderFullPath, FString Ext)
{
	if (RootFolderFullPath.Len() < 1) return false;

	FPaths::NormalizeDirectoryName(RootFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();

	if (!Ext.Contains(TEXT("*")))
	{
		if (Ext == "")
		{
			Ext = "*.*";
		}
		else
		{
			Ext = (Ext.Left(1) == ".") ? "*" + Ext : "*." + Ext;
		}
	}

	const FString FinalPath = RootFolderFullPath + "/" + Ext;

	FileManager.FindFiles(Files, *FinalPath, true, false);
	return true;
}

bool UClassicFunctionLibrary::LoadStringFile(FString& Result, FString FullFilePath)
{
	return FFileHelper::LoadFileToString(Result, *FullFilePath);
}

static EImageFormat GetJoyImageFormat(EClassicImageFormat JoyFormat)
{
	switch (JoyFormat)
	{
	case EClassicImageFormat::JPG: return EImageFormat::JPEG;
	case EClassicImageFormat::PNG: return EImageFormat::PNG;
	case EClassicImageFormat::BMP: return EImageFormat::BMP;
	case EClassicImageFormat::ICO: return EImageFormat::ICO;
	case EClassicImageFormat::EXR: return EImageFormat::EXR;
	case EClassicImageFormat::ICNS: return EImageFormat::ICNS;
	}
	return EImageFormat::JPEG;
}

static TextureFilter GetTextureFilter(EClassicTextureFilter Filter)
{
	switch (Filter)
	{
	case EClassicTextureFilter::NEAREST: return TF_Nearest;
	case EClassicTextureFilter::DEFAULT: return TF_Default;
	case EClassicTextureFilter::MAX: return TF_MAX;
	case EClassicTextureFilter::BILINEAR: return TF_Bilinear;
	case EClassicTextureFilter::TRILINEAR: return TF_Trilinear;
	}
	return TF_Nearest;
}

EClassicImageFormat UClassicFunctionLibrary::GetFormatImage(const FString& FullFilePath)
{
	if (FullFilePath.Contains("png"))
	{
		return EClassicImageFormat::PNG;
	}
	if (FullFilePath.Contains("jpg"))
	{
		return EClassicImageFormat::JPG;
	}
	if (FullFilePath.Contains("bmp"))
	{
		return EClassicImageFormat::BMP;
	}
	if (FullFilePath.Contains("ico"))
	{
		return EClassicImageFormat::ICO;
	}
	return EClassicImageFormat::PNG;
}

UTexture2D* UClassicFunctionLibrary::LoadTexture2DFromFile(const FString& FullFilePath, EClassicImageFormat ImageFormat, EClassicTextureFilter Filter, int32& Width, int32& Height)
{
	if (!FPaths::FileExists(FullFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("File not found. (%s)"), *FullFilePath);
		return nullptr;
	}
	IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

	UTexture2D* NewTexture = nullptr;
	TArray64<uint8> Buffer;

	if (FFileHelper::LoadFileToArray(Buffer, *FullFilePath))
	{
		EPixelFormat PixelFormat = PF_Unknown;
		EImageFormat Format = GetJoyImageFormat(ImageFormat);


		if (Format != EImageFormat::Invalid)
		{
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);

			int32 BitDepth = 0;

			// Create texture
			if (ImageWrapper->SetCompressed((void*)Buffer.GetData(), Buffer.Num()))
			{
				PixelFormat = PF_Unknown;
				ERGBFormat RGBFormat = ERGBFormat::Invalid;

				BitDepth = ImageWrapper->GetBitDepth();

				Width = ImageWrapper->GetWidth();
				Height = ImageWrapper->GetHeight();

				if (BitDepth == 16)
				{
					PixelFormat = PF_FloatRGBA;
					RGBFormat = ERGBFormat::RGBAF;
				}
				else if (BitDepth == 8)
				{
					PixelFormat = PF_B8G8R8A8;
					RGBFormat = ERGBFormat::BGRA;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Bit depth is unsupported. (%d)"), BitDepth);
					return nullptr;
				}

				TArray64<uint8> UncompressedData;
				ImageWrapper->GetRaw(RGBFormat, BitDepth, UncompressedData);

				// Load texture
				const FString& BaseFilename = FPaths::GetBaseFilename(FullFilePath);

				// Load texture
				NewTexture = CreateUniqueTransient(Width, Height, PixelFormat, *BaseFilename);
				if (NewTexture)
				{
					NewTexture->bNotOfflineProcessed = true;
					uint8* MipData = static_cast<uint8*>(NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

					// Bulk data was already allocated for the correct size when we called CreateTransient above
					FMemory::Memcpy(MipData, UncompressedData.GetData(), NewTexture->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize());

					// Copy texture and update
					NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
					NewTexture->Filter = GetTextureFilter(Filter);
					NewTexture->CompressionSettings = TC_EditorIcon;
					NewTexture->UpdateResource();
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Couldn't determine the file format"));
		}
	}
	return NewTexture;
}

void UClassicFunctionLibrary::AsyncLoadTexture2DFromFile(FLoadImageDelegate Out, const FString FullFilePath, int32 Index, EClassicImageFormat ImageFormat, EClassicTextureFilter Filter)
{
	AsyncTask(ENamedThreads::AnyThread, [=]()
	{
		if (!FPaths::FileExists(FullFilePath))
		{
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				Out.ExecuteIfBound(nullptr, Index, false);
			});
			UE_LOG(LogTemp, Warning, TEXT("File Not Exists. %s"), *FullFilePath);
			return;
		}
		IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

		TArray64<uint8> Buffer;

		if (FFileHelper::LoadFileToArray(Buffer, *FullFilePath))
		{
			EPixelFormat PixelFormat = PF_Unknown;
			EImageFormat Format = GetJoyImageFormat(ImageFormat);
			int32 Width = 0;
			int32 Height = 0;

			if (Format != EImageFormat::Invalid)
			{
				TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);

				int32 BitDepth = 0;

				// Create texture
				if (ImageWrapper->SetCompressed((void*)Buffer.GetData(), Buffer.Num()))
				{
					PixelFormat = PF_Unknown;
					ERGBFormat RGBFormat = ERGBFormat::Invalid;

					BitDepth = ImageWrapper->GetBitDepth();

					Width = ImageWrapper->GetWidth();
					Height = ImageWrapper->GetHeight();

					if (BitDepth == 16)
					{
						PixelFormat = PF_FloatRGBA;
						RGBFormat = ERGBFormat::RGBAF;
					}
					else if (BitDepth == 8)
					{
						PixelFormat = PF_B8G8R8A8;
						RGBFormat = ERGBFormat::BGRA;
					}
					else
					{
						AsyncTask(ENamedThreads::GameThread, [=]()
						{
							Out.ExecuteIfBound(nullptr, Index, false);
						});
						UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Bit depth is unsupported. (%d)"), BitDepth);
						return;
					}

					TArray64<uint8> UncompressedData;
					ImageWrapper->GetRaw(RGBFormat, BitDepth, UncompressedData);

					AsyncTask(ENamedThreads::GameThread, [=]()
					{
						const FString& BaseFilename = FPaths::GetBaseFilename(FullFilePath);
						bool bSuccessfull = false;

						// Load texture
						UTexture2D* NewTexture = CreateUniqueTransient(Width, Height, PixelFormat, *BaseFilename);
						if (NewTexture)
						{
							NewTexture->NeverStream = true;
							NewTexture->bNotOfflineProcessed = true;

							uint8* MipData = static_cast<uint8*>(NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
							// Bulk data was already allocated for the correct size when we called CreateTransient above
							FMemory::Memcpy(MipData, UncompressedData.GetData(), NewTexture->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize());
							NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

							// Copy texture set parameters and update
							NewTexture->Filter = GetTextureFilter(Filter);
							NewTexture->CompressionSettings = TC_EditorIcon;
							NewTexture->UpdateResource();
							bSuccessfull = true;
						}
						// execute function 
						Out.ExecuteIfBound(NewTexture, Index, bSuccessfull);
					});
					return; // early return successful!!
				}
			}
		}
		// execute function
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			Out.ExecuteIfBound(nullptr, Index, false);
		});
	});
}

UTexture2D* UClassicFunctionLibrary::CreateUniqueTransient(int32 Width, int32 Height, EPixelFormat PixelFormat, const FName BaseFilename)
{
	if (Width > 0 && Height > 0 && (Width % GPixelFormats[PixelFormat].BlockSizeX) == 0 && (Height % GPixelFormats[PixelFormat].BlockSizeY) == 0)
	{
		UTexture2D* NewTexture = NewObject<UTexture2D>(
			GetTransientPackage(),
			MakeUniqueObjectName(GetTransientPackage(), UTexture2D::StaticClass(), BaseFilename),
			RF_Public | RF_Transient
		);

		NewTexture->SetPlatformData(new FTexturePlatformData());
		NewTexture->GetPlatformData()->SizeX = Width;
		NewTexture->GetPlatformData()->SizeY = Height;
		NewTexture->GetPlatformData()->PixelFormat = PixelFormat;

		// Allocate first mipmap.
		const int32 NumBlocksX = Width / GPixelFormats[PixelFormat].BlockSizeX;
		const int32 NumBlocksY = Height / GPixelFormats[PixelFormat].BlockSizeY;
		FTexture2DMipMap* Mip = new FTexture2DMipMap();
		NewTexture->GetPlatformData()->Mips.Add(Mip);
		Mip->SizeX = Width;
		Mip->SizeY = Height;
		Mip->BulkData.Lock(LOCK_READ_WRITE);
		Mip->BulkData.Realloc(NumBlocksX * NumBlocksY * GPixelFormats[PixelFormat].BlockBytes);
		Mip->BulkData.Unlock();
		return NewTexture;
	}
	return nullptr;
}

void UClassicFunctionLibrary::CreateProc(int32& ProcessId, FString FullPath, TArray<FString> CommandlineArgs, bool Detach, bool Hidden, int32 Priority, FString OptionalWorkingDirectory)
{
	FString Args = "";
	if (CommandlineArgs.Num() > 1)
	{
		Args = CommandlineArgs[0];
		for (int32 v = 1; v < CommandlineArgs.Num(); v++)
		{
			Args += " " + CommandlineArgs[v];
		}
	}
	else if (CommandlineArgs.Num() > 0)
	{
		Args = CommandlineArgs[0];
	}

	uint32 NeedBPUINT32 = 0;
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*FullPath,
		*Args,
		Detach, //* @param bLaunchDetached		if true, the new process will have its own window
		false, //* @param bLaunchHidded			if true, the new process will be minimized in the task bar
		Hidden, //* @param bLaunchReallyHidden	if true, the new process will not have a window or be in the task bar
		&NeedBPUINT32,
		Priority,
		(OptionalWorkingDirectory != "") ? *OptionalWorkingDirectory : nullptr, //const TCHAR* OptionalWorkingDirectory, 
		nullptr
	);

	//Not sure what to do to expose UINT32 to BP
	ProcessId = NeedBPUINT32;
}

FString UClassicFunctionLibrary::FormatDateToView(FString DateXml)
{
	if (DateXml.IsEmpty()) return TEXT("");

	const TArray<FString> DateFormat = UKismetStringLibrary::GetCharacterArrayFromString(DateXml);
	FString Date;
	FString Mounth;
	FString Year;


	for (int32 i = 0; i < DateFormat.Num(); i++)
	{
		if (i < 4)
		{
			Year += DateFormat[i];
		}
		else if (i < 6)
		{
			Mounth += DateFormat[i];
		}
		else if (i < 8)
		{
			Date += DateFormat[i];
		}
	}
	//2022-12-31
	return Year + TEXT("-") + Mounth + TEXT("-") + Date;
}

FString UClassicFunctionLibrary::FormatDateTimeToView(FString DateXml)
{
	const TArray<FString> DateFormat = UKismetStringLibrary::GetCharacterArrayFromString(DateXml);
	FString Date = FormatDateToView(DateXml);

	FString Hour;
	FString Minute;

	for (int32 i = 9; i < DateFormat.Num(); i++)
	{
		if (i < 11)
		{
			Hour += DateFormat[i];
		}
		else if (i < 13)
		{
			Minute += DateFormat[i];
		}
	}
	//2022-12-31 22:30
	return Date + TEXT("  ") + Hour + TEXT(":") + Minute;
}

FDateTime UClassicFunctionLibrary::GetXMLDateTime(FString DateXml)
{
	return FDateTime();
}

FString UClassicFunctionLibrary::FormatDateToXml()
{
	const FDateTime DateTime = FDateTime::Now();
	return DateTime.ToString(TEXT("%Y%m%dT%H%M%S"));
}

FColor UClassicFunctionLibrary::HexToColor(FString HexString)
{
	return FColor::FromHex(HexString);
}

FString UClassicFunctionLibrary::ColorToHex(FColor Color)
{
	return Color.ToHex();
}

FString UClassicFunctionLibrary::GetProjectVersion()
{
	if (!GConfig) return "";

	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}

FString UClassicFunctionLibrary::GetProjectName()
{
	if (!GConfig) return "";

	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectName"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}


FString UClassicFunctionLibrary::ReadRegistryValue(const FString& KeyName, const FString& ValueName)
{
	HKEY hKey;
	LONG Result = RegOpenKeyExW(HKEY_CURRENT_USER, *KeyName, 0, KEY_READ, &hKey);
	if (Result != ERROR_SUCCESS)
	{
		// Handle error 
	}
	TCHAR Buffer[MAX_PATH];
	DWORD BufferSize = sizeof(Buffer);
	HRESULT hResult = RegQueryValueEx(hKey, *ValueName, 0, nullptr, reinterpret_cast<LPBYTE>(Buffer), &BufferSize);
	if (hResult != ERROR_SUCCESS)
	{
		// Handle error 
	}
	std::wstring Bu = Buffer;
	FString Value = FString(Bu.c_str());
	return Value;
}

bool UClassicFunctionLibrary::IsRunningSteamApp(const FString& AppID)
{
	const FString KeyName = TEXT("Software\\Valve\\Steam\\Apps\\") + AppID;
	const FString ValueName = TEXT("Running");
	const FString Value = ReadRegistryValue(KeyName, ValueName);

	if (Value.Len() == 0) return false;
	if (Value[0] == 1) return true;

	return false;
}

UUserWidget* UClassicFunctionLibrary::GetFirstWidgetOfClass(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly)
{
	if (!WidgetClass || !WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return nullptr;
	}

	UUserWidget* ResultWidget = nullptr;
	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		UUserWidget* LiveWidget = *Itr;

		// Skip any widget that's not in the current world context.
		if (LiveWidget->GetWorld() != World)
		{
			continue;
		}

		// Skip any widget that is not a child of the class specified.
		if (!LiveWidget->GetClass()->IsChildOf(WidgetClass))
		{
			continue;
		}

		if (TopLevelOnly)
		{
			//only add top level widgets
			if (LiveWidget->IsInViewport())
			{
				ResultWidget = LiveWidget;
				break;
			}
		}
		else
		{
			ResultWidget = LiveWidget;
			break;
		}
	}

	return ResultWidget;
}

void UClassicFunctionLibrary::DefineEffects(USoundBase* SelectSound, USoundBase* NavigateSound)
{
	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		UUserWidget* LiveWidget = *Itr;

		if (LiveWidget->GetClass()->ImplementsInterface(UMusicInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("name class %s "), *LiveWidget->GetName());
			IMusicInterface* MyInterface = Cast<IMusicInterface>(LiveWidget);
			if (MyInterface)
			{
				MyInterface->EffectSound(SelectSound, NavigateSound);
			}
		}
	}
}

void FindFileFolders(TArray<FString>& Files, FString FullFilePath, FString Extension, const bool Recursive, const bool FilesSearch, const bool FolderSearch)
{
	FPaths::NormalizeDirectoryName(FullFilePath);
	IFileManager& FileManager = IFileManager::Get();

	FString FinalExtension = TEXT("*");

	if (Extension != TEXT("") && Extension != TEXT("*") && Extension != TEXT("*.*") && FilesSearch)
	{
		FinalExtension = TEXT("*.") + Extension;
	}

	if (Recursive)
	{
		FileManager.FindFilesRecursive(Files, *FullFilePath, *FinalExtension, FilesSearch, FolderSearch);
		for (int32 i = 0; i < Files.Num(); i++)
		{
			FString Path = FullFilePath + TEXT("/");
			Files[i] = Files[i].Replace(*Path, TEXT(""));
		}
	}
	else
	{
		const FString FinalPath = FullFilePath / FinalExtension;
		FileManager.FindFiles(Files, *FinalPath, FilesSearch, FolderSearch);
	}
}

bool UClassicFunctionLibrary::GetFolders(TArray<FString>& Folders, FString FullFilePath, const bool Recursive)
{
	FindFileFolders(Folders, FullFilePath, TEXT("*"), Recursive, false, true);

	for (int i = 0; i < Folders.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Folders Found: %s"), *Folders[i]);
	}

	return Folders.Num() > 0;
}

bool UClassicFunctionLibrary::GetFiles(TArray<FString>& Files, FString FullFilePath, TArray<FString> Extensions, const bool Recursive)
{
	for (int32 i = 0; i < Extensions.Num(); i++)
	{
		TArray<FString> FilesLocal;
		FindFileFolders(FilesLocal, FullFilePath, Extensions[i], Recursive, true, false);
		Files.Append(FilesLocal);
	}

	Files.Sort();

	for (int i = 0; i < Files.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("File Found: %s"), *Files[i]);
	}

	return Files.Num() > 0;
}

bool UClassicFunctionLibrary::LoadTextureToAsset(const FString& FullFilePath, EClassicImageFormat ImageFormat, EClassicTextureFilter Filter, int32& Width, int32& Height, const FString FileName)
{
	bool bSaved = false;

	if (!FPaths::FileExists(FullFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("File not found. (%s)"), *FullFilePath);
		return false;
	}
	IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

	UTexture2D* NewTexture = nullptr;
	TArray64<uint8> Buffer;

	if (FFileHelper::LoadFileToArray(Buffer, *FullFilePath))
	{
		EPixelFormat PixelFormat = PF_Unknown;
		EImageFormat Format = GetJoyImageFormat(ImageFormat);

		if (Format != EImageFormat::Invalid)
		{
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);
			int32 BitDepth = 0;

			// Create texture
			if (ImageWrapper->SetCompressed((void*)Buffer.GetData(), Buffer.Num()))
			{
				PixelFormat = PF_Unknown;
				ERGBFormat RGBFormat = ERGBFormat::Invalid;
				BitDepth = ImageWrapper->GetBitDepth();

				Width = ImageWrapper->GetWidth();
				Height = ImageWrapper->GetHeight();

				if (BitDepth == 16)
				{
					PixelFormat = PF_FloatRGBA;
					RGBFormat = ERGBFormat::RGBAF;
				}
				else if (BitDepth == 8)
				{
					PixelFormat = PF_B8G8R8A8;
					RGBFormat = ERGBFormat::BGRA;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Bit depth is unsupported. (%d)"), BitDepth);
					return false;
				}

				TArray64<uint8> UncompressedData;
				ImageWrapper->GetRaw(RGBFormat, BitDepth, UncompressedData);

				// Load texture
				const FString& BaseFilename = FPaths::GetBaseFilename(FullFilePath);

				// Load texture
				FString PackageName = TEXT("/Game/ProceduralTextures/");
				PackageName += FileName;
				UPackage* Package = CreatePackage(*PackageName);
				Package->FullyLoad();

				CreateTexture2DToAsset(Width, Height, PixelFormat, *FileName, NewTexture, Package);

				if (NewTexture)
				{
					NewTexture->AddToRoot();
					NewTexture->bNotOfflineProcessed = true;
					uint8* MipData = static_cast<uint8*>(NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

					// Bulk data was already allocated for the correct size when we called CreateTexture2DToAsset above
					FMemory::Memcpy(MipData, UncompressedData.GetData(), NewTexture->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize());

					// Copy texture and update
					NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
					NewTexture->Filter = GetTextureFilter(Filter);
					NewTexture->NeverStream = true;
					NewTexture->CompressionSettings = TC_EditorIcon;
#if WITH_EDITORONLY_DATA
					NewTexture->Source.Init(Width, Height, 1, 1, TSF_BGRA8, UncompressedData.GetData());
#endif

					NewTexture->UpdateResource();
					Package->MarkPackageDirty();
					FAssetRegistryModule::AssetCreated(NewTexture);

					const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
					FSavePackageArgs SaveArgs;
					SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
					SaveArgs.SaveFlags = SAVE_NoError;
					SaveArgs.Error = GError;
					SaveArgs.bForceByteSwapping = true;
					SaveArgs.bWarnOfLongFilename = true;
					bSaved = UPackage::SavePackage(Package, NewTexture, *PackageFileName, SaveArgs);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error creating texture. Couldn't determine the file format"));
			return false;
		}
	}

	return bSaved;
}

void UClassicFunctionLibrary::CreateTexture2DToAsset(int32 InSizeX, int32 InSizeY, EPixelFormat InFormat, const FName InName, UTexture2D*& NewTexture, UPackage*& Package)
{
	if (InSizeX > 0 && InSizeY > 0 && (InSizeX % GPixelFormats[InFormat].BlockSizeX) == 0 && (InSizeY % GPixelFormats[InFormat].BlockSizeY) == 0)
	{
		NewTexture = NewObject<UTexture2D>(Package, InName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

		NewTexture->SetPlatformData(new FTexturePlatformData());
		NewTexture->GetPlatformData()->SizeX = InSizeX;
		NewTexture->GetPlatformData()->SizeY = InSizeY;
		NewTexture->GetPlatformData()->PixelFormat = InFormat;

		// Allocate first mipmap.
		const int32 NumBlocksX = InSizeX / GPixelFormats[InFormat].BlockSizeX;
		const int32 NumBlocksY = InSizeY / GPixelFormats[InFormat].BlockSizeY;
		FTexture2DMipMap* Mip = new FTexture2DMipMap();
		NewTexture->GetPlatformData()->Mips.Add(Mip);
		Mip->SizeX = InSizeX;
		Mip->SizeY = InSizeY;
		Mip->BulkData.Lock(LOCK_READ_WRITE);
		Mip->BulkData.Realloc(NumBlocksX * NumBlocksY * GPixelFormats[InFormat].BlockBytes);
		Mip->BulkData.Unlock();
	}
}

void UClassicFunctionLibrary::GameSettingsInit()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(60.0f);
	Settings->SetVSyncEnabled(true);
	Settings->SetResolutionScaleValueEx(25);
	Settings->SetViewDistanceQuality(0);
	Settings->SetAntiAliasingQuality(0);
	Settings->SetPostProcessingQuality(0);
	Settings->SetShadowQuality(0);
	Settings->SetGlobalIlluminationQuality(0);
	Settings->SetReflectionQuality(0);
	Settings->SetTextureQuality(0);
	Settings->SetVisualEffectQuality(0);
	Settings->SetFoliageQuality(0);
	Settings->SetShadingQuality(0);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}

void UClassicFunctionLibrary::GameSettingsRunning()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFrameRateLimit(30.0f);
	Settings->ApplySettings(true);
	Settings->SaveSettings();
}

bool UClassicFunctionLibrary::GetVisibilityWidget(const UWidget* Widget)
{
	return Widget->GetVisibility() != ESlateVisibility::Collapsed &&
		Widget->GetVisibility() != ESlateVisibility::Hidden &&
		Widget->GetVisibility() != ESlateVisibility::HitTestInvisible;
}
