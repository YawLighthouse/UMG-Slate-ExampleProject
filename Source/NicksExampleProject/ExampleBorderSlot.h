// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Margin.h"
#include "Components/PanelSlot.h"
#include "ExampleBorderSlot.generated.h"

class SExampleBorder;
class UExampleBorder;

/**
 * 
 */
UCLASS()
class NICKSEXAMPLEPROJECT_API UExampleBorderSlot : public UPanelSlot
{
	GENERATED_BODY()	
	
public:

	UFUNCTION(BlueprintCallable, Category="Layout|Border Slot")
	void SetPadding(FMargin InPadding);

	UFUNCTION(BlueprintCallable, Category="Layout|Border Slot")
    void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category="Layout|Border Slot")
    void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);
	
	// UPanelSlot interface
	/** This is basically the visuals if the slate widget pointer is valid */
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	/** Builds the underlying slot for the slate border. */	
	void BuildSlot(TSharedRef<SExampleBorder> InBorder);

	/** This is an inherited function that is intended to be overriden so as to tell slate widgets to reset. */
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR

	// UObject interface	
	/** This is a regular UObject function that fires when a property is changed */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// End of UObject interface

#endif
	
protected:
	
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, Category="Layout|Border Slot")
	FMargin Padding = FMargin(4, 2);

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, Category="Layout|Border Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = HAlign_Fill;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, Category="Layout|Border Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment = VAlign_Fill;

private:

	/** A pointer to the border to allow us to adjust the size, padding...etc at runtime. */
	TWeakPtr<SExampleBorder> Border;

	/** Allow this class to access its private variables which is just the pointer to the slate widget */
	friend UExampleBorder;
	
};
