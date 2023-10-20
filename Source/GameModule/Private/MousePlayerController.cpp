// Fill out your copyright notice in the Description page of Project Settings.


#include "MousePlayerController.h"
#include "GameFramework/PlayerInput.h"

AMousePlayerController::AMousePlayerController()
	: Super() {
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bShowMouseCursor = true;
}

void AMousePlayerController::BeginPlay() {
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}


bool AMousePlayerController::InputKey(const FInputKeyParams& Params) {
	const bool result = Super::InputKey(Params);

	if (result && Params.Key.IsMouseButton()) {
		switch (Params.Event) {
			case(IE_DoubleClick): {
				FVector HitLocation;
				AActor* Actor;
				if (GetMouseHit(HitLocation, Actor)) {
					DoubleClickActor(HitLocation, Actor, Params.Key);
				}
				break;
			}
			case(IE_Pressed): {
				FVector HitLocation;
				AActor* Actor;
				if (GetMouseHit(HitLocation, Actor)) {
					PressActor(HitLocation, Actor, Params.Key);
				} else {
					ReleaseActor(FVector(), m_PressedActor, Params.Key);
				}
				break;
			}
			case(IE_Released): {
				ReleaseActor(FVector(), m_PressedActor, Params.Key);
				break;
			}
		}
	}
	
	return result;
}

bool AMousePlayerController::GetMouseHit(FVector& Result, AActor*& Actor) {
	FVector StartTrace, EndTrace;
	FRotator dummy;
	GetPlayerViewPoint(StartTrace, dummy);
	EndTrace = GetMousePosition();
	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s, %s"), *StartTrace.ToString(), *EndTrace.ToString()));
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility)) {
		Result = HitResult.Location;
		Actor = HitResult.GetActor();
		return true;
	}
	return false;
}

FVector AMousePlayerController::GetMousePosition() {
	FVector MouseLocation, MouseDirection;
	if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection)) {
		return MouseLocation + (MouseDirection * 10000);
	} else {
		return FVector();
	}
}

void AMousePlayerController::PressActor(const FVector& Location, AActor* Actor, FKey Key) {
	ReleaseActor(Location, m_PressedActor, Key);
	m_PressedActor = Actor;
	OnPressed.Broadcast(Location, m_PressedActor, Key);
}

void AMousePlayerController::DoubleClickActor(const FVector& Location, AActor* Actor, FKey Key) {
	PressActor(Location, Actor, Key);
	OnDoubleClick.Broadcast(Location, Actor, Key);
}

void AMousePlayerController::ReleaseActor(const FVector& Location, AActor* Actor, FKey Key) {
	if (Actor) {
		OnReleased.Broadcast(Location, Actor, Key);
		m_PressedActor = nullptr;
	}
}