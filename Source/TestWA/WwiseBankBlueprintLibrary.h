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

    /** Gameplay-facing audio flow. Bank names are owned centrally in C++. */
    UFUNCTION(BlueprintCallable, Category = "Wwise|Audio Flow", meta = (DisplayName = "Enter Menu Audio"))
    static bool EnterMenuAudio();

    UFUNCTION(BlueprintCallable, Category = "Wwise|Audio Flow", meta = (DisplayName = "Enter Level Audio"))
    static bool EnterLevelAudio();

    UFUNCTION(BlueprintCallable, Category = "Wwise|Audio Flow", meta = (DisplayName = "Leave Level Audio"))
    static bool LeaveLevelAudio();

    UFUNCTION(BlueprintCallable, Category = "Wwise|Audio Flow", meta = (DisplayName = "Leave Menu Audio"))
    static bool LeaveMenuAudio();

    UFUNCTION(
        BlueprintCallable,
        Category = "Wwise|SoundBank|Debug",
        meta = (DisplayName = "Load Wwise Bank By Name")
    )
    static bool LoadWwiseBankByName(const FString& BankName);

    UFUNCTION(
        BlueprintCallable,
        Category = "Wwise|SoundBank|Debug",
        meta = (DisplayName = "Unload Wwise Bank By Name")
    )
    static bool UnloadWwiseBankByName(const FString& BankName);
};
