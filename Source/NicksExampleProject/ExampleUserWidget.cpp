// Fill out your copyright notice in the Description page of Project Settings.

#include "ExampleUserWidget.h"
#include "ExampleBorder.h"

UExampleUserWidget::UExampleUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UExampleUserWidget::ChangeBorderColor(FLinearColor InColor)
{
	// Even if you know it should be connected to something valid, its good to be safe about it
	if(Border)
	{
		// Set the border's brush color itself
		Border->SetBrushColor(InColor);
	}
}

void UExampleUserWidget::NativeConstruct()
{
	// Create our timer delegate to handle calling the correct function
	FTimerDelegate TimerDelegate;
	// Create our timer handle to automatically call the correct function
	FTimerHandle TimerHandle;
	// Setup our delegate to call the function and feed in a red color
	TimerDelegate.BindUFunction(this, "ChangeBorderColor", FLinearColor::Red);
	// Set the timer to run
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.0f, false);

	// We finished what we needed to happen, now we notify BP that construct has occured
	Super::NativeConstruct();
}
