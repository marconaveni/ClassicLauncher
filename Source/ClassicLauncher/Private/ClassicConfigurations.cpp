// Copyright 2022 Marco Naveni. All Rights Reserved.


#include "ClassicConfigurations.h"
#include "MainInterface.h"
#include "EngineUtils.h"


void UClassicConfigurations::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (TObjectIterator<UMainInterface> ObjectIterator ; ObjectIterator; ++ObjectIterator)
	{
		MainInterfaceReference = *ObjectIterator;
		UE_LOG(LogTemp, Warning, TEXT("Reference UMainInterface Founds: %s "), *MainInterfaceReference->GetName());
	}
}
