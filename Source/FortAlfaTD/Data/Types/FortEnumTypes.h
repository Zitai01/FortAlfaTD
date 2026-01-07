#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok,
	YesNo,
	OkCancel,
	Unknown UMETA(hIDDEN),
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Comfirmed,
	Cancelled,
	Closed,
	Unknown UMETA(hIDDEN),
};