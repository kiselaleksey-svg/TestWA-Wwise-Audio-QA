#include "WwiseBankBlueprintLibrary.h"
#include "AudioBankProfile.h"
#include "AkAudioDevice.h"
#include "Wwise/WwiseResourceCooker.h"
#include "Wwise/WwiseResourceLoader.h"
#include "Wwise/CookedData/WwiseLocalizedSoundBankCookedData.h"
#include "Wwise/Loaded/WwiseLoadedSoundBank.h"

namespace
{
    TMap<FString, FWwiseLoadedSoundBankPtr> LoadedBanks;

   
}
bool UWwiseBankBlueprintLibrary::LoadAudioBankProfile(const UAudioBankProfile* Profile)
{
    if (!Profile)
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] Audio Bank Profile is null."));
        return false;
    }

    if (Profile->SoundBankNames.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[WwiseBankBridge] Audio Bank Profile '%s' has no banks."), *Profile->GetName());
        return false;
    }

    bool bAllBanksLoaded = true;

    for (const FName& BankName : Profile->SoundBankNames)
    {
        if (!LoadWwiseBankByName(BankName.ToString()))
        {
            bAllBanksLoaded = false;
        }
    }

    return bAllBanksLoaded;
}

bool UWwiseBankBlueprintLibrary::UnloadAudioBankProfile(const UAudioBankProfile* Profile)
{
    if (!Profile)
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] Audio Bank Profile is null."));
        return false;
    }

    bool bAllBanksUnloaded = true;

    for (const FName& BankName : Profile->SoundBankNames)
    {
        if (!UnloadWwiseBankByName(BankName.ToString()))
        {
            bAllBanksUnloaded = false;
        }
    }

    return bAllBanksUnloaded;
}




bool UWwiseBankBlueprintLibrary::LoadWwiseBankByName(const FString& BankName)
{
    if (BankName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] Bank name is empty."));
        return false;
    }

    if (LoadedBanks.Contains(BankName))
    {
        UE_LOG(LogTemp, Log, TEXT("[WwiseBankBridge] Bank '%s' is already loaded."), *BankName);
        return true;
    }

    IWwiseResourceCooker* ResourceCooker = IWwiseResourceCooker::GetDefault();
    const FWwiseResourceLoaderPtr ResourceLoader = FWwiseResourceLoader::Get();
    if (!ResourceCooker || !ResourceLoader)
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] Wwise resource services are unavailable."));
        return false;
    }

    FAkAudioDevice* AkAudioDevice = FAkAudioDevice::Get();
    if (!AkAudioDevice)
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] AkAudioDevice is unavailable."));
        return false;
    }

    const uint32 BankId = AkAudioDevice->GetShortIDFromString(BankName);
    FWwiseLocalizedSoundBankCookedData CookedData;
    if (!ResourceCooker->GetSoundBankCookedData(CookedData, TEXT("WwiseAudio"), FWwiseObjectInfo(BankId, BankName)))
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] Could not prepare cooked data for bank '%s'."), *BankName);
        return false;
    }

    FWwiseLoadedSoundBankPtr LoadedBank = ResourceLoader->LoadSoundBank(CookedData);
    if (!LoadedBank)
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] Resource loader failed to load bank '%s'."), *BankName);
        return false;
    }

    LoadedBanks.Add(BankName, LoadedBank);
    UE_LOG(LogTemp, Log, TEXT("[WwiseBankBridge] Loaded managed SoundBank '%s'."), *BankName);

    return true;
}


bool UWwiseBankBlueprintLibrary::UnloadWwiseBankByName(const FString& BankName)
{
    FWwiseLoadedSoundBankPtr* LoadedBank = LoadedBanks.Find(BankName);
    if (!LoadedBank)
    {
        UE_LOG(LogTemp, Warning, TEXT("[WwiseBankBridge] Bank '%s' is not loaded."), *BankName);
        return false;
    }

    const FWwiseResourceLoaderPtr ResourceLoader = FWwiseResourceLoader::Get();
    if (!ResourceLoader)
    {
        UE_LOG(LogTemp, Error, TEXT("[WwiseBankBridge] Wwise resource loader is unavailable."));
        return false;
    }

    ResourceLoader->UnloadSoundBank(MoveTemp(*LoadedBank));
    LoadedBanks.Remove(BankName);
    UE_LOG(LogTemp, Log, TEXT("[WwiseBankBridge] Unloaded managed SoundBank '%s'."), *BankName);

    return true;
}
