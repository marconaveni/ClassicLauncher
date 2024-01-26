// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "XMLDateTime.h"

#include "Kismet/KismetInternationalizationLibrary.h"


FXMLDateTime::FXMLDateTime(const FString& NewValue)
{
	DateTime = StringToDateTime(NewValue);
	SetDateStringValue();
}

FXMLDateTime::FXMLDateTime(const FDateTime& NewValue)
{
	DateTime = NewValue;
	SetDateStringValue();
}

FXMLDateTime::FXMLDateTime(const TCHAR* NewValue)
{
	DateTime = StringToDateTime(NewValue);
	SetDateStringValue();
}

void FXMLDateTime::operator=(const FString& NewValue)
{
	DateTime = StringToDateTime(NewValue);
	SetDateStringValue();
}


void FXMLDateTime::operator=(const FDateTime& NewValue)
{
	DateTime = NewValue;
	SetDateStringValue();
}

void FXMLDateTime::operator=(const TCHAR* NewValue)
{
	DateTime = StringToDateTime(NewValue);
	SetDateStringValue();
}

void FXMLDateTime::SetDateStringValue()
{
	DateStringValue = DateTime.ToString(TEXT("%Y%m%dT%H%M%S"));
}

FString FXMLDateTime::FormatDateTime()
{
	const FString CurrentLanguage = UKismetInternationalizationLibrary::GetCurrentCulture();
	const FCulturePtr LanguageCulture = FInternationalization::Get().GetCulture(CurrentLanguage);
	const FText Text = FText::AsDateTime(DateTime, EDateTimeStyle::Short, EDateTimeStyle::Medium, FText::GetInvariantTimeZone(), LanguageCulture);
	return Text.ToString();
}

FDateTime FXMLDateTime::StringToDateTime(FString Value)
{
	int32 Year = 1, Month = 1, Day = 1;
	int32 Hour = 0, Minute = 0, Second = 0;
	if (Value.Len() != 15)
	{
		return FDateTime(Year, Month, Day, Hour, Minute, Second);
	}

	FString Split = Value;
	Split.MidInline(0, 4);
	Year = FCString::Atoi(*Split);

	Split = Value;
	Split.MidInline(4, 2);
	Month = FCString::Atoi(*Split);

	Split = Value;
	Split.MidInline(6, 2);
	Day = FCString::Atoi(*Split);

	Split = Value;
	Split.MidInline(9, 2);
	Hour = FCString::Atoi(*Split);

	Split = Value;
	Split.MidInline(11, 2);
	Minute = FCString::Atoi(*Split);

	Split = Value;
	Split.MidInline(13, 2);
	Second = FCString::Atoi(*Split);
	
	return FDateTime(Year, Month, Day, Hour, Minute, Second);
}
