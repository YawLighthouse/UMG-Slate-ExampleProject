// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleBorder.h"
#include "ExampleBorderSlot.h"
#include "SExampleBorder.h"
#include "ObjectEditorUtils.h"
#include "Slate/SlateBrushAsset.h"

#define LOCTEXT_NAMESPACE "UMG"

UExampleBorder::UExampleBorder()    
{
	bIsVariable = false;
	bShowEffectWhenDisabled = true;
}

void UExampleBorder::SetContentColorAndOpacity(FLinearColor InContentColorAndOpacity)
{
	ContentColorAndOpacity = InContentColorAndOpacity;
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetColorAndOpacity(InContentColorAndOpacity);
	}
}

void UExampleBorder::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetPadding(InPadding);
	}
}

void UExampleBorder::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetHAlign(InHorizontalAlignment);
	}
}

void UExampleBorder::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetVAlign(InVerticalAlignment);
	}
}

void UExampleBorder::SetBrushColor(FLinearColor InBrushColor)
{
	BrushColor = InBrushColor;
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetBorderBackgroundColor(InBrushColor);
	}
}

void UExampleBorder::SetBrush(const FSlateBrush& InBrush)
{
	Background = InBrush;
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetBorderImage(&Background);
	}
}

void UExampleBorder::SetBrushFromAsset(USlateBrushAsset* InAsset)
{
	Background = InAsset ? InAsset->Brush : FSlateBrush();
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetBorderImage(&Background);
	}
}

void UExampleBorder::SetBrushFromTexture(UTexture2D* InTexture)
{
	Background.SetResourceObject(InTexture);
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetBorderImage(&Background);
	}
}

void UExampleBorder::SetBrushFromMaterial(UMaterialInterface* InMaterial)
{
	if (!InMaterial)
	{
		UE_LOG(LogSlate, Log, TEXT("UBorder::SetBrushFromMaterial.  Incoming material is null"));
	}

	Background.SetResourceObject(InMaterial);
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetBorderImage(&Background);
	}
}

UMaterialInstanceDynamic* UExampleBorder::GetDynamicMaterial()
{
	UMaterialInterface* Material = nullptr;

	// Grab the material from the background brush
	UObject* Resource = Background.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	// If that material is valid
	if (Material)
	{
		// Try to get a dynamic material from it
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		// If the dynamic material is null
		if (!DynamicMaterial)
		{
			// Then create a new one and update our background brush to use it
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Background.SetResourceObject(DynamicMaterial);

			// Update our slate widget to use it
			if (MyBorder.IsValid())
			{
				MyBorder->SetBorderImage(&Background);
			}
		}

		// And finally, return that dynamic material
		return DynamicMaterial;
	}

	// Otherwise return null
	return nullptr;
}

void UExampleBorder::SetDesiredSizeScale(FVector2D InScale)
{
	DesiredSizeScale = InScale;
	if (MyBorder.IsValid())
	{
		MyBorder->SetDesiredSizeScale(InScale);
	}
}

void UExampleBorder::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	// Setting up our attribute bindings and values
	const TAttribute<FLinearColor> ContentColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, ContentColorAndOpacity);
	const TAttribute<FSlateColor> BrushColorBinding = OPTIONAL_BINDING_CONVERT(FLinearColor, BrushColor, FSlateColor, ConvertLinearColorToSlateColor);
	// This one is binding the attribute to a function rather than a value
	const TAttribute<const FSlateBrush*> ImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Background, const FSlateBrush*, ConvertImage);

	// Telling our slate widget to update its values to those attributes
	// by calling those functions that we made in our slate widget!
	MyBorder->SetPadding(Padding);
	MyBorder->SetBorderBackgroundColor(BrushColorBinding);
	MyBorder->SetColorAndOpacity(ContentColorAndOpacityBinding);

	MyBorder->SetBorderImage(ImageBinding);
	
	MyBorder->SetDesiredSizeScale(DesiredSizeScale);
	MyBorder->SetShowEffectWhenDisabled(bShowEffectWhenDisabled != 0);

	// Binding our delegates with our slate widget's delegates
	MyBorder->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
	MyBorder->SetOnMouseButtonUp(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonUp));
	MyBorder->SetOnMouseMove(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseMove));
	MyBorder->SetOnMouseDoubleClick(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseDoubleClick));
}

void UExampleBorder::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	// This is pretty simple, just reset the pointer which handles immediate garbage collection and such
	MyBorder.Reset();
}

void UExampleBorder::PostLoad()
{
	Super::PostLoad();

	// If we have any children
	if (GetChildrenCount() > 0)
	{
		// Get our panel slot
		if (UPanelSlot* PanelSlot = GetContentSlot())
		{
			// Try to get our custom border slot
			UExampleBorderSlot* BorderSlot = Cast<UExampleBorderSlot>(PanelSlot);
			// If its null
			if (!BorderSlot)
			{
				// Then make a new one and manually connect it up
				BorderSlot = NewObject<UExampleBorderSlot>(this);
				// We have to update its content to ours
				BorderSlot->Content = GetContentSlot()->Content;
				// Tell that content that its slot is our newly created border slot
				BorderSlot->Content->Slot = BorderSlot;
				// And then update our parent variable's Slots array at index 0 to this slot
				Slots[0] = BorderSlot;
			}
		}
	}
}

#if WITH_EDITOR

void UExampleBorder::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// This is the same way we did it in our slot class
	static bool IsReentrant = false;

	if ( !IsReentrant )
	{
		IsReentrant = true;

		if ( PropertyChangedEvent.Property )
		{
			static const FName PaddingName("Padding");
			static const FName HorizontalAlignmentName("HorizontalAlignment");
			static const FName VerticalAlignmentName("VerticalAlignment");

			const FName PropertyName = PropertyChangedEvent.Property->GetFName();
			
			if (UExampleBorderSlot* BorderSlot = Cast<UExampleBorderSlot>(GetContentSlot()))
			{
				if (PropertyName == PaddingName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, PaddingName, BorderSlot, PaddingName);
				}
				else if (PropertyName == HorizontalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, HorizontalAlignmentName, BorderSlot, HorizontalAlignmentName);
				}
				else if (PropertyName == VerticalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, VerticalAlignmentName, BorderSlot, VerticalAlignmentName);
				}
			}
		}

		IsReentrant = false;
	}
}

const FText UExampleBorder::GetPaletteCategory()
{
	// We're placing our widget in the common category of the palette list
	return LOCTEXT("Common", "Common");
}

#endif

UClass* UExampleBorder::GetSlotClass() const
{
	// Returns the class of our slot
	return UExampleBorderSlot::StaticClass();
}

void UExampleBorder::OnSlotAdded(UPanelSlot* InSlot)
{
	// Copy the content properties into the new slot so that it matches what has been setup
	// so far by the user.
	UExampleBorderSlot* BorderSlot = CastChecked<UExampleBorderSlot>(InSlot);
	BorderSlot->Padding = Padding;
	BorderSlot->HorizontalAlignment = HorizontalAlignment;
	BorderSlot->VerticalAlignment = VerticalAlignment;

	// Add the child to the live slot if it already exists
	if ( MyBorder.IsValid() )
	{
		// Construct the underlying slot.
		BorderSlot->BuildSlot(MyBorder.ToSharedRef());
	}
}

void UExampleBorder::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyBorder.IsValid() )
	{
		MyBorder->SetContent(SNullWidget::NullWidget);
	}
}

TSharedRef<SWidget> UExampleBorder::RebuildWidget()
{
	// Creates our slate widget, SNew is the keyword for new widget essentially
	MyBorder = SNew(SExampleBorder);

	// If we have any children
	if ( GetChildrenCount() > 0 )
	{
		// Then tell our to build itself while passing in our slate widget ptr
		Cast<UExampleBorderSlot>(GetContentSlot())->BuildSlot(MyBorder.ToSharedRef());
	}

	// Return our created slate widget
	return MyBorder.ToSharedRef();
}

FReply UExampleBorder::HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if ( OnMouseButtonDownEvent.IsBound() )
	{
		return OnMouseButtonDownEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

FReply UExampleBorder::HandleMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if ( OnMouseButtonUpEvent.IsBound() )
	{
		return OnMouseButtonUpEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

FReply UExampleBorder::HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if ( OnMouseMoveEvent.IsBound() )
	{
		return OnMouseMoveEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

FReply UExampleBorder::HandleMouseDoubleClick(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if ( OnMouseDoubleClickEvent.IsBound() )
	{
		return OnMouseDoubleClickEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

const FSlateBrush* UExampleBorder::ConvertImage(TAttribute<FSlateBrush> InImageAsset) const
{
	// Get a modifiable version of this
	UExampleBorder* MutableThis = const_cast<UExampleBorder*>(this);
	// Set its background at the same time
	MutableThis->Background = InImageAsset.Get();

	// Return that background
	return &Background;
}

#undef LOCTEXT_NAMESPACE
