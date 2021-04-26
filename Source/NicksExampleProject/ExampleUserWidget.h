// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExampleUserWidget.generated.h"

class UExampleBorder;

// We mark this class abstract because all user widget's in C++ have to be inherited from
// so this is to help avoid creating instances when we subclass the widget in Blueprint.

/**
 * 
 */
UCLASS(abstract)
class NICKSEXAMPLEPROJECT_API UExampleUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** Ctor */
	UExampleUserWidget(const FObjectInitializer& ObjectInitializer);

	/** Handle's changing the border color to whatever color is inputted in. */
	UFUNCTION(BlueprintCallable, Category = "Example User Widget")
	void ChangeBorderColor(FLinearColor InColor);
	
	/**
	 * Our required border widget(don't worry the editor/engine will connect them for us),
	 * we use BindWidget instead of BindWidgetOptional for tutorial purposes and so I don't forget to add it in the designer.	
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UExampleBorder* Border;
	
protected:

	/** Overriding our native construction function */
	virtual void NativeConstruct() override;
	
};

