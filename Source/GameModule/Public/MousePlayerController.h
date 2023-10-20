// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MousePlayerController.generated.h"

DECLARE_EVENT_ThreeParams(AMousePlayerController, FOnPressed, const FVector&, AActor*, FKey)
DECLARE_EVENT_ThreeParams(AMousePlayerController, FOnDoubleClick, const FVector&, AActor*, FKey)
DECLARE_EVENT_ThreeParams(AMousePlayerController, FOnReleased, const FVector&, AActor*, FKey)

/**
 * 
 */
UCLASS(HideDropdown)
class GAMEMODULE_API AMousePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnPressed OnPressed;
	FOnDoubleClick OnDoubleClick;
	FOnReleased OnReleased;

	AMousePlayerController();
	virtual void BeginPlay() override;

	// Redirect the mouse actions
	bool InputKey(const FInputKeyParams& Params);
	// Finds an actor or a position under the mouse cursor
	bool GetMouseHit(FVector& Result, AActor*& Actor);
	// Returns the position in world space of the mouse cursor
	FVector GetMousePosition();

private:

	// Maintained to dispatch a OnReleased event when the mouse is not over the actor when it's actually released
	UPROPERTY()
	AActor* m_PressedActor;

	void PressActor(const FVector& Location, AActor* Actor, FKey Key);
	void DoubleClickActor(const FVector& Location, AActor* Actor, FKey Key);
	void ReleaseActor(const FVector& Location, AActor* Actor, FKey Key);
};
