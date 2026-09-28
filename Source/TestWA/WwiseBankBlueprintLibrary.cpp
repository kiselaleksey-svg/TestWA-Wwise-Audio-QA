#include "WwiseBankBlueprintLibrary.h"
#include "AkAudioDevice.h"
#include "Wwise/WwiseResourceCooker.h"
#include "Wwise/WwiseResourceLoader.h"
#include "Wwise/CookedData/WwiseLocalizedSoundBankCookedData.h"
#include "Wwise/Loaded/WwiseLoadedSoundBank.h"

namespace
{
    TMap<FString, FWwiseLoadedSoundBankPtr> LoadedBanks;

    const FString MenuBankName(TEXT("MenuBank"));
    const FString LevelBankName(TEXT("LevelBank"));

    bool UnloadIfLoaded(const FString& BankName)
    {
        return !LoadedBanks.Contains(BankName) || UWwiseBankBlueprintLibrary::UnloadWwiseBankByName(BankName);
    }
}

bool UWwiseBankBlueprintLibrary::EnterMenuAudio()
{
    // Level-specific data must not stay resident after returning to the menu.
    return UnloadIfLoaded(LevelBankName) && LoadWwiseBankByName(MenuBankName);
}

bool UWwiseBankBlueprintLibrary::EnterLevelAudio()
{
    // Keep MenuBank loaded: pause/settings UI can remain available during gameplay.
    return LoadWwiseBankByName(LevelBankName);
}

bool UWwiseBankBlueprintLibrary::LeaveLevelAudio()
{
    return UnloadIfLoaded(LevelBankName);
}

bool UWwiseBankBlueprintLibrary::LeaveMenuAudio()
{
    return UnloadIfLoaded(MenuBankName);
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
