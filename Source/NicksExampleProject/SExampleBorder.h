// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class NICKSEXAMPLEPROJECT_API SExampleBorder : public SCompoundWidget
{
public:

	// These are basically constructor parameters for SExampleBorder when calling SNew on them
	// This also allows for us to do bindings as well within SNew
	SLATE_BEGIN_ARGS(SExampleBorder)
		: _HAlign( HAlign_Fill )
    	, _VAlign( VAlign_Fill )
    	, _Padding( FMargin(2.0f) )
		// FCoreStyle is only used for in engine assets, we do this here because we want to recreate our brush
		// in normal projects do not do this!
		, _BorderImage( FCoreStyle::Get().GetBrush( "Border" ) )
        , _ContentScale( FVector2D(1,1) )
        , _DesiredSizeScale( FVector2D(1,1) )
		, _ShowEffectWhenDisabled( true )
		, _ColorAndOpacity( FLinearColor(1,1,1,1) )
		, _BorderBackgroundColor( FLinearColor::White )
		, _ForegroundColor( FSlateColor::UseForeground() )
		{ }

	// Declaring the widget argument to add to this class's child slot
	SLATE_DEFAULT_SLOT( FArguments, Content )

	// Overriding parent argument's and attributes
	SLATE_ARGUMENT( EHorizontalAlignment, HAlign )
    SLATE_ARGUMENT( EVerticalAlignment, VAlign )
    SLATE_ATTRIBUTE( FMargin, Padding )

	SLATE_EVENT( FPointerEventHandler, OnMouseButtonDown )
    SLATE_EVENT( FPointerEventHandler, OnMouseButtonUp )
    SLATE_EVENT( FPointerEventHandler, OnMouseMove )
    SLATE_EVENT( FPointerEventHandler, OnMouseDoubleClick )
	
	SLATE_ATTRIBUTE(const FSlateBrush*, BorderImage)
	SLATE_ATTRIBUTE( FVector2D, ContentScale )
	SLATE_ATTRIBUTE( FVector2D, DesiredSizeScale )	
	/** Whether or not to show the disabled effect when this border is disabled */
	SLATE_ATTRIBUTE( bool, ShowEffectWhenDisabled )
	
	/** ColorAndOpacity is the color and opacity of content in the border */
    SLATE_ATTRIBUTE( FLinearColor, ColorAndOpacity )
    /** BorderBackgroundColor refers to the actual color and opacity of the supplied border image.*/
    SLATE_ATTRIBUTE( FSlateColor, BorderBackgroundColor )
    /** The foreground color of text and some glyphs that appear as the border's content. */
    SLATE_ATTRIBUTE( FSlateColor, ForegroundColor )
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs from the SLATE_BEGIN_ARGS/SLATE_END_ARGS parameters */
	void Construct(const FArguments& InArgs);

	/**
	* Sets the content for this border
	*
	* @param	InContent	The widget to use as content for the border
	*/
	virtual void SetContent( TSharedRef< SWidget > InContent );

	/**
	* Gets the content for this border
	*
	* @return The widget used as content for the border
	*/
	const TSharedRef< SWidget >& GetContent() const;

	/** Clears out the content for the border */
	void ClearContent();
	
	/** Sets the color and opacity of the background image of this border. */
    void SetBorderBackgroundColor(const TAttribute<FSlateColor>& InColorAndOpacity);
   
    /** Set the desired size scale multiplier */
    void SetDesiredSizeScale(const TAttribute<FVector2D>& InDesiredSizeScale);
    
    /** See HAlign argument */
    void SetHAlign(EHorizontalAlignment HAlign);
   
    /** See VAlign argument */
    void SetVAlign(EVerticalAlignment VAlign);
   
    /** See Padding attribute */
    void SetPadding(const TAttribute<FMargin>& InPadding);
   
    /** See ShowEffectWhenDisabled attribute */
    void SetShowEffectWhenDisabled(const TAttribute<bool>& InShowEffectWhenDisabled);
   
    /** See BorderImage attribute */
    void SetBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage);

	// SWidget interface
	virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
	virtual bool ComputeVolatility() const override;
	// End of SWidget interface
	
protected:

	// Begin SWidget overrides.
	virtual FVector2D ComputeDesiredSize(float) const override;
	// End SWidget overrides.
	
	// This is a invalidatable slate brush(fancier version of Slate Brush that allows caching of the brush)
	// would have been the same as TAttribute<FSlateBrush> if it wasn't
	FInvalidatableBrushAttribute BorderImage;
	
	TAttribute<FSlateColor> BorderBackgroundColor;
	TAttribute<FVector2D> DesiredSizeScale;
	
	/** Whether or not to show the disabled effect when this border is disabled */
	TAttribute<bool> ShowDisabledEffect;
	
};
