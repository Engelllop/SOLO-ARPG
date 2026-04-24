#include "Profile/EMBERVEILAdminAuth.h"

namespace
{
	static const TCHAR* GAdminPassword = TEXT("89722729");

	bool ExtractAdminIndex(const FString& Tail, int32& OutIndex)
	{
		if (Tail.IsEmpty())
		{
			return false;
		}

		for (int32 i = 0; i < Tail.Len(); ++i)
		{
			if (!FChar::IsDigit(Tail[i]))
			{
				return false;
			}
		}

		const int32 Value = FCString::Atoi(*Tail);
		const FString RoundTrip = FString::FromInt(Value);
		if (RoundTrip != Tail)
		{
			return false;
		}

		if (Value < 0 || Value > 10)
		{
			return false;
		}

		OutIndex = Value;
		return true;
	}
}

bool EMBERVEILAdminAuth::IsAdminUsername(const FString& Username, FString& OutCanonicalName)
{
	OutCanonicalName.Reset();

	FString S = Username;
	S.TrimStartAndEndInline();
	if (S.Len() < 6)
	{
		return false;
	}

	FString Prefix = S.Left(5);
	if (!Prefix.Equals(TEXT("Admin"), ESearchCase::IgnoreCase))
	{
		return false;
	}

	const FString Tail = S.Mid(5);
	int32 Index = INDEX_NONE;
	if (!ExtractAdminIndex(Tail, Index))
	{
		return false;
	}

	OutCanonicalName = FString::Printf(TEXT("Admin%d"), Index);
	return true;
}

bool EMBERVEILAdminAuth::VerifyAdminPassword(const FString& Password)
{
	FString P = Password;
	P.TrimStartAndEndInline();
	return P.Equals(GAdminPassword, ESearchCase::CaseSensitive);
}
