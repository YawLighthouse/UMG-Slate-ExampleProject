// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "Misc/Attribute.h"
#include "Layout/Margin.h"
#include "Styling/SlateBrush.h"
#include "ExampleBorder.generated.h"

class SExampleBorder;
class USlateBrushAsset;

/**
 * 
 */
UCLASS()
class NICKSEXAMPLEPROJECT_API UExampleBorder : public UContentWidget
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	UExampleBorder();
   
    /** The alignment of the content horizontally. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Content")
    TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = HAlign_Fill;
   
    /** The alignment of the content vertically. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Content")
    TEnumAsByte<EVerticalAlignment> VerticalAlignment = VAlign_Fill;
   
    /** Whether or not to show the disabled effect when this border is disabled */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
    uint8 bShowEffectWhenDisabled : 1;
   
    /** Color and opacity multiplier of content in the border */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Content", meta=( sRGB="true" ))
    FLinearColor ContentColorAndOpacity = FLinearColor::White;
   
    /** A bindable delegate for the ContentColorAndOpacity. */
    UPROPERTY()
    FGetLinearColor ContentColorAndOpacityDelegate;
   
    /** The padding area between the slot and the content it contains. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Content")
    FMargin Padding = FMargin(4, 2);
   
    /** Brush to drag as the background */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( DisplayName="Brush" ))
    FSlateBrush Background;
   
    /** A bindable delegate for the Brush. */
    UPROPERTY()
    FGetSlateBrush BackgroundDelegate;
   
    /** Color and opacity of the actual border image */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( sRGB="true" ))
    FLinearColor BrushColor = FLinearColor::White;
   
    /** A bindable delegate for the BrushColor. */
    UPROPERTY()
    FGetLinearColor BrushColorDelegate;
   
    /**
     * Scales the computed desired size of this border and its contents. Useful
     * for making things that slide open without having to hard-code their size.
     * Note: if the parent widget is set up to ignore this widget's desired size,
     * then changing this value will have no effect.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
    FVector2D DesiredSizeScale = FVector2D(1, 1);
    
    /*************************DELEGATES***************************/
    
    UPROPERTY(EditAnywhere, Category=Events, meta=( IsBindableEvent="True" ))
	FOnPointerEvent OnMouseButtonDownEvent;
	
	UPROPERTY(EditAnywhere, Category=Events, meta=( IsBindableEvent="True" ))
	FOnPointerEvent OnMouseButtonUpEvent;
	
	UPROPERTY(EditAnywhere, Category=Events, meta=( IsBindableEvent="True" ))
	FOnPointerEvent OnMouseMoveEvent;
	
	UPROPERTY(EditAnywhere, Category=Events, meta=( IsBindableEvent="True" ))
	FOnPointerEvent OnMouseDoubleClickEvent;

	/*************************END OF DELEGATES***************************/

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetContentColorAndOpacity(FLinearColor InContentColorAndOpacity);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetPadding(FMargin InPadding);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetBrushColor(FLinearColor InBrushColor);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetBrush(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetBrushFromAsset(USlateBrushAsset* InAsset);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetBrushFromTexture(UTexture2D* InTexture);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetBrushFromMaterial(UMaterialInterface* InMaterial);

	UFUNCTION(BlueprintCallable, Category="Appearance")
    UMaterialInstanceDynamic* GetDynamicMaterial();

	/**
	* Sets the DesireSizeScale of this border.
	*
	* @param InScale    The X and Y multipliers for the desired size
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetDesiredSizeScale(FVector2D InScale);

	//~ Begin UWidget Interface	
	/** Here we bind our delegates and properties to the slate widget */
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface	
	/** Here we reset the slate widget also if its not pointed to by any other classes, garbage collected */
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//~ Begin UObject Interface
	/** Here we handle any deprecations(not needed in our case) and telling our slot to update */
	virtual void PostLoad() override;
	//~ End UObject Interface

// These are editor only inherited functions
#if WITH_EDITOR
	
	//~ Begin UObject Interface
	/** Here we are keeping everything in sync with our slot the same way we did in our slot class */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	/** This is just for cosmetic's but it is required, the category of where to place this widget in the palette list */
	virtual const FText GetPaletteCategory() override;
#endif

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	// End UPanelWidget

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface
	
	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	FReply HandleMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	FReply HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	FReply HandleMouseDoubleClick(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	/** Translates the bound brush data and assigns it to the cached brush used by this widget. */
	const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;
	
	// Our slate pointer
	TSharedPtr<SExampleBorder> MyBorder;

	// Declare that we're gonna implement a property binding between the slate widget and this value
	PROPERTY_BINDING_IMPLEMENTATION(FLinearColor, ContentColorAndOpacity)	
};
