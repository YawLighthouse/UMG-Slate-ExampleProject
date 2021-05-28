// Fill out your copyright notice in the Description page of Project Settings.


#include "SExampleBorder.h"

#include "SlateOptMacros.h"

// This is a newer macro that is meant to help build the project faster
// so you won't see this in most regular widgets because they haven't been updated in a long time.
// What this macro specifically does, is it indicates to the compiler that we do not want it to attempt to optimize this code block
// apparently this macro should be placed on large slate widget declarations as the heavy nesting can cause the compiler to spend a lot of time
// attempting to optimize the single expression
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

static FName SExampleBorderTypeName("SExampleBorder");

void SExampleBorder::Construct(const FArguments& InArgs)
{
	// Only do this if we're exactly an SExampleBorder
	if (GetType() == SExampleBorderTypeName)
	{
		SetCanTick(false);
		bCanSupportFocus = false;
	}

	// Set our variables to what our inputted arguments are
	ContentScale = InArgs._ContentScale;
	ColorAndOpacity = InArgs._ColorAndOpacity;
	DesiredSizeScale = InArgs._DesiredSizeScale;

	ShowDisabledEffect = InArgs._ShowEffectWhenDisabled;

	BorderImage = InArgs._BorderImage;
	BorderBackgroundColor = InArgs._BorderBackgroundColor;
	ForegroundColor = InArgs._ForegroundColor;

	// Set our delegates but only if the inputted arguments has a bound delegate
	if (InArgs._OnMouseButtonDown.IsBound())
	{
		SetOnMouseButtonDown(InArgs._OnMouseButtonDown);
	}
	if (InArgs._OnMouseButtonUp.IsBound())
	{
		SetOnMouseButtonUp(InArgs._OnMouseButtonUp);
	}
	if (InArgs._OnMouseMove.IsBound())
	{
		SetOnMouseMove(InArgs._OnMouseMove);
	}
	if (InArgs._OnMouseDoubleClick.IsBound())
	{
		SetOnMouseDoubleClick(InArgs._OnMouseDoubleClick);
	}
	
	ChildSlot // Here we are getting the variable ChildSlot
    .HAlign(InArgs._HAlign) // and then setting its HAlign variable to our inputted argument
    .VAlign(InArgs._VAlign) // and then setting its VAlign variable to our inputted argument
    .Padding(InArgs._Padding) // and then setting its padding variable to our inputted argument
    [ // These brackets are to signal a child widget is being modified, like your getting a value out of an array by doing MyArray[i]
    	InArgs._Content.Widget // Here's where we're setting that slot's widget to our inputted argument's content widget
    ]; // And we're done
}

void SExampleBorder::SetContent(TSharedRef<SWidget> InContent)
{
	ChildSlot // Get the ChildSlot variable
	[ // Brackets to notify we're doing stuff to the widget that's tied to this slot
		InContent // Set that widget to be InContent
	];
}

const TSharedRef<SWidget>& SExampleBorder::GetContent() const
{
	// We just get the ChildSlot variable and get the widget from that slot
	return ChildSlot.GetWidget();
}

void SExampleBorder::ClearContent()
{
	// What this does is it returns the widget that was detached, and also sets its widget to a SNullWidget
	ChildSlot.DetachWidget(); // I'VE ABANDONED MY CHILD!!!!
}

void SExampleBorder::SetBorderBackgroundColor(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	SetAttribute(BorderBackgroundColor, InColorAndOpacity, EInvalidateWidgetReason::Paint);
}

void SExampleBorder::SetDesiredSizeScale(const TAttribute<FVector2D>& InDesiredSizeScale)
{
	SetAttribute(DesiredSizeScale, InDesiredSizeScale, EInvalidateWidgetReason::Layout);
}

void SExampleBorder::SetHAlign(EHorizontalAlignment HAlign)
{
	if (ChildSlot.HAlignment != HAlign)
	{
		ChildSlot.HAlignment = HAlign;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void SExampleBorder::SetVAlign(EVerticalAlignment VAlign)
{
	if (ChildSlot.VAlignment != VAlign)
	{
		ChildSlot.VAlignment = VAlign;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void SExampleBorder::SetPadding(const TAttribute<FMargin>& InPadding)
{
	SetAttribute(ChildSlot.SlotPadding, InPadding, EInvalidateWidgetReason::Layout);
}

void SExampleBorder::SetShowEffectWhenDisabled(const TAttribute<bool>& InShowEffectWhenDisabled)
{
	SetAttribute(ShowDisabledEffect, InShowEffectWhenDisabled, EInvalidateWidgetReason::Paint);
}

void SExampleBorder::SetBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage)
{
	BorderImage.SetImage(*this, InBorderImage);
}

int32 SExampleBorder::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Get our brush
	const FSlateBrush* BrushResource = BorderImage.Get();

	// Check if this widget is enabled/disabled
    const bool bEnabled = ShouldBeEnabled(bParentEnabled);

	// Only draw the box if we're allowed to draw anything
    if ( BrushResource && BrushResource->DrawAs != ESlateBrushDrawType::NoDrawType )
    {
    	// Get the disabled effect value
    	const bool bShowDisabledEffect = ShowDisabledEffect.Get();
    	// Figure our which effect to draw
    	const ESlateDrawEffect DrawEffects = (bShowDisabledEffect && !bEnabled) ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;
    	// This creates a primitive box ontop of this widget
    	FSlateDrawElement::MakeBox(
                   OutDrawElements,
                   LayerId,
                   AllottedGeometry.ToPaintGeometry(),
                   BrushResource,
                   DrawEffects,
                   BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * BorderBackgroundColor.Get().GetColor(InWidgetStyle)
               );
    }
   
    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bEnabled );
}

bool SExampleBorder::ComputeVolatility() const
{
	// Check to make sure everything is properly bound to a value
	return BorderImage.IsBound()
	|| BorderBackgroundColor.IsBound()
	|| DesiredSizeScale.IsBound()
	|| ShowDisabledEffect.IsBound();
}

FVector2D SExampleBorder::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// If you're getting an error regarding the layout scale multiplier, thats because the parameter wasn't setup with a name initially in the .h of base class
	return DesiredSizeScale.Get() * SCompoundWidget::ComputeDesiredSize(LayoutScaleMultiplier);
}

// This is the end of that newer macro at the top of the page
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
