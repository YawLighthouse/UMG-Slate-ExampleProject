// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleBorderSlot.h"
#include "ExampleBorder.h"
#include "SExampleBorder.h"
#include "ObjectEditorUtils.h"

void UExampleBorderSlot::SetPadding(FMargin InPadding)
{
	// Get the Border UWidget and set its padding, pretty much routing the function call to the actual widget rather than handling it within the slot
	CastChecked<UExampleBorder>(Parent)->SetPadding(InPadding);
}

void UExampleBorderSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	// Get the Border UWidget and set its horizontal alignment, pretty much routing the function call to the actual widget rather than handling it within the slot
	CastChecked<UExampleBorder>(Parent)->SetHorizontalAlignment(InHorizontalAlignment);
}

void UExampleBorderSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	// Get the Border UWidget and set its vertical alignment, pretty much routing the function call to the actual widget rather than handling it within the slot
	CastChecked<UExampleBorder>(Parent)->SetVerticalAlignment(InVerticalAlignment);
}

void UExampleBorderSlot::SynchronizeProperties()
{
	// If the slate pointer is valid
	if ( Border.IsValid() )
	{
		// Then update padding and alignments
		SetPadding(Padding);
		SetHorizontalAlignment(HorizontalAlignment);
		SetVerticalAlignment(VerticalAlignment);
	}
}

void UExampleBorderSlot::BuildSlot(TSharedRef<SExampleBorder> InBorder)
{
	// Cache(save) our slate border widget
	Border = InBorder;

	// Border.Pin() is basically getting the actual pointer from the weak_ptr template
	
	Border.Pin()->SetPadding(Padding);
	Border.Pin()->SetHAlign(HorizontalAlignment);
	Border.Pin()->SetVAlign(VerticalAlignment);

	// Either use a valid widget or a null widget
	Border.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UExampleBorderSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	// This really doesn't do anything but its good practice to call the parent version incase that changes
	Super::ReleaseSlateResources(bReleaseChildren);

	// Tell the weak ptr to either handle destroying the border or reset its pointer to null
	Border.Reset();
}

#if WITH_EDITOR

void UExampleBorderSlot::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// This is to avoid running multiple times in one frame in editor
	static bool IsReentrant = false;

	if (!IsReentrant)
	{
		IsReentrant = true;

		if (PropertyChangedEvent.Property)
		{
			static const FName PaddingName("Padding");
			static const FName HorizontalAlignmentName("HorizontalAlignment");
			static const FName VerticalAlignmentName("VerticalAlignment");

			const FName PropertyName = PropertyChangedEvent.Property->GetFName();

			// Double check our parent is the example border widget
			if (UExampleBorder* ParentBorder = CastChecked<UExampleBorder>(Parent))
			{
				// Check for if the property changed was something we need to account for
				// MigratePropertyValue basically copying data between our parent widget and this object to keep them in sync for certain values while in editor
				if (PropertyName == PaddingName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, PaddingName, ParentBorder, PaddingName);
				}
				else if (PropertyName == HorizontalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, HorizontalAlignmentName, ParentBorder, HorizontalAlignmentName);
				}
				else if (PropertyName == VerticalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, VerticalAlignmentName, ParentBorder, VerticalAlignmentName);
				}
			}
		}

		// We're done checking so we can run through it again
		IsReentrant = false;
	}
}

#endif
