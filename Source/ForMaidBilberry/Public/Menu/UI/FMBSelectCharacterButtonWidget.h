// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBSelectCharacterButtonWidget.generated.h"

class UButton;
class UTextBlock;
class USizeBox;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBSelectCharacterButtonWidget : public UUserWidget
{
    GENERATED_BODY()
    /*public:
        FOnCharacterMeshSelectedSignature OnCharacterMeshSelected;

        void SetCharacterData(const FCharacterData& Data);
        FCharacterData GetCharacterData() const { return CharacterData; }

        void IsCharacterMeshSelected(bool IsSelected);

    protected:
        UPROPERTY(meta = (BindWidget))
        UButton* SelectCharacterMeshButton;

        UPROPERTY(meta = (BindWidget))
        UTextBlock* CharacterNameTextBlock;

        UPROPERTY(meta = (BindWidget))
        USizeBox* CharacterNameSizeBox;

        virtual void NativeOnInitialized() override;

    private:
        FCharacterData CharacterData;

        FVector2D NormalCharacterNameSizeBox = {400.0f, 90.0f};
        FVector2D SelectedCharacterNameSizeBox = {450.0f, 120.0f};

        UFUNCTION()
        void OnCharacterSelectButtonClicked();*/
};
