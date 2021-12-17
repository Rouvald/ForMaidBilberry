// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBLevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class USizeBox;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBLevelItemWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    FOnLevelSelectedSignature OnLevelSelected;

    void SetLevelData(const FLevelData& Data);
    FLevelData GetLevelData() const { return LevelData; }

    void IsLevelSelected( bool IsSelected);

    /*float GetNormalSizeBox () const {return NormalSizeBox;}
    float GetSelectedSizeBox () const {return SelectedSizeBox;}*/

protected:
    UPROPERTY(meta = (BindWidget))
    USizeBox* LevelIconSizeBox;
    
    /*UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelNameTextBlock;*/

    UPROPERTY(meta = (BindWidget))
    UImage* LevelImage = nullptr;

    /*UPROPERTY(meta = (BindWidget))
    UImage* LevelFrameImage;*/

    UPROPERTY(meta = (BindWidget))
    UButton* SelectLevelButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelDescriptionTextBlock;

    virtual void NativeOnInitialized() override;

private:
    FLevelData LevelData;

    float NormalSizeBox = 150.0f;
    float SelectedSizeBox = 300.0f;

    UFUNCTION()
    void OnLevelItemClicked();
};
