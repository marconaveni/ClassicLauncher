// Copyright 2024 Marco Naveni. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "XMLDateTime.generated.h"

USTRUCT(BlueprintType)
struct FXMLDateTime
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FString DateStringValue;

	UPROPERTY()
	FDateTime DateTime;

	void SetDateTime(const FString& NewValue);
	
	void SetDateTime(const FDateTime& NewValue);
	
public:
	
	FXMLDateTime()
	{
		DateTime = FDateTime(1, 1, 1);
		SetDateStringValue();
	}
	
	void operator=(const FString& NewValue);

	void operator=(const FDateTime& NewValue);

	void operator=(const TCHAR* NewValue);

	FORCEINLINE FString ToString()
	{
		return (!DateStringValue.Equals(TEXT("00010101T000000"))) ? DateStringValue : TEXT("");
	}

	FORCEINLINE FDateTime ToDateTime() const
	{
		return DateTime;
	}

	FORCEINLINE bool operator<(const FXMLDateTime& A) const
	{
		return DateTime < A.DateTime;
	}

	FORCEINLINE bool operator>(const FXMLDateTime& A) const
	{
		return DateTime > A.DateTime;
	}

	FORCEINLINE bool operator==(const FXMLDateTime& A) const
	{
		return DateTime == A.DateTime;
	}

	FString FormatDateTime(const bool bIsTime = true);

private:
	FDateTime StringToDateTime(FString Value);

	void SetDateStringValue();
};
