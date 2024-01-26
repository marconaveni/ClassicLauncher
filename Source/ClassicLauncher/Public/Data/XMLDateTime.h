// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"


struct FXMLDateTime
{

private:
	
	FString DateStringValue;
	FDateTime DateTime;

public:

	FORCEINLINE FXMLDateTime()
	{
		DateTime = FDateTime(1,1,1);
		SetDateStringValue();
	}

	FXMLDateTime(const FString& NewValue);

	FXMLDateTime(const FDateTime& NewValue);

	FXMLDateTime(const TCHAR* NewValue);
	
	void operator=(const FString& NewValue);
	
	void operator=(const FDateTime& NewValue);

	void operator=(const TCHAR* NewValue);

	FORCEINLINE FString ToString()
	{
		return (!DateStringValue.Equals(TEXT("00010101T000000"))) ? DateStringValue : TEXT("");
	}

	FORCEINLINE FDateTime ToDateTime()
	{
		return DateTime;
	}

	FString FormatDateTime();

private:
	
	FDateTime StringToDateTime(FString Value);

	void SetDateStringValue();
};


