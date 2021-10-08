// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBGameHUD.h"
#include "Engine/Canvas.h"

void AFMBGameHUD::DrawHUD()
{
    //DrawTwoLine();
}

void AFMBGameHUD::DrawTwoLine()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 5.0f;
    const float LineTHickness = 2.0f;
    const FColor ColorLine = FColor::Purple;

    DrawLine(Center.Min - HalfLineSize, Center.Max - HalfLineSize, Center.Min + HalfLineSize, Center.Max + HalfLineSize, ColorLine,
        LineTHickness);
    DrawLine(Center.Min - HalfLineSize, Center.Max + HalfLineSize, Center.Min + HalfLineSize, Center.Max - HalfLineSize, ColorLine,
        LineTHickness);
}
