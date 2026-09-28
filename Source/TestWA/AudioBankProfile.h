#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AudioBankProfile.generated.h"


UCLASS(BlueprintType)
class TESTWA_API UAudioBankProfile : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Банки, которые должны быть загружены в данном аудиоконтексте.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wwise|SoundBanks")
	TArray<FName> SoundBankNames;

};