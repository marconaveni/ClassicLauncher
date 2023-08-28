// Copyright 2023 Marco Naveni. All Rights Reserved.

#pragma once

#include "DoOnce.generated.h"


USTRUCT(BlueprintType)
struct FDoOnce
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bDoOnce;

	FORCEINLINE FDoOnce(bool bStartClosed = true)
	{
		bDoOnce = bStartClosed;
	}

	FORCEINLINE void Reset() 
	{ 
		bDoOnce = true; 
	}
	
	FORCEINLINE bool Execute() 
	{ 
		if (bDoOnce)
		{
			bDoOnce = false;
			return true;
		}
		return false;
	}

};


