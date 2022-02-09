// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBSelectCharacterButtonWidget.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBSelectCharacterWidget.generated.h"

class UVerticalBox;
class UFMBGameInstance;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBSelectCharacterWidget : public UUserWidget
{
    GENERATED_BODY()

    /*
    protected:
        UPROPERTY(meta = (BindWidget))
        UVerticalBox* CharacterNamesBox;

        UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> SelectCharacterButtonWidgetClass;

        virtual void NativeOnInitialized() override;

    private:
        UPROPERTY()
        TArray<UFMBSelectCharacterButtonWidget*> SelectCharacterButtonWidgets;

        void InitCharacterMesh();
        //void OnCharacterMeshSelected(const FCharacterData& Data);
        UFMBGameInstance* GetFMBGameInstance() const;*/
};
