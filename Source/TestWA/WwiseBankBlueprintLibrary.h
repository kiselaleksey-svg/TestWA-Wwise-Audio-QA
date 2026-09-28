#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AudioBankProfile.h"
#include "WwiseBankBlueprintLibrary.generated.h"

UCLASS()
class TESTWA_API UWwiseBankBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Wwise|Audio Profiles")
    static bool LoadAudioBankProfile(const UAudioBankProfile* Profile);

    UFUNCTION(BlueprintCallable, Category = "Wwise|Audio Profiles")
    static bool UnloadAudioBankProfile(const UAudioBankProfile* Profile);

private:
    static bool LoadWwiseBankByName(const FString& BankName);
    static bool UnloadWwiseBankByName(const FString& BankName);
};