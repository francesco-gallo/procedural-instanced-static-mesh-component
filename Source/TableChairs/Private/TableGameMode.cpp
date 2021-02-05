// Fill out your copyright notice in the Description page of Project Settings.


#include "TableGameMode.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "MousePlayerController.h"
#include "AsyncFunctionLibrary.h"

ATableGameMode::ATableGameMode() {
	PlayerControllerClass = AMousePlayerController::StaticClass();
	m_PoolSize = 5;
}

void ATableGameMode::BeginPlay() {
	GEngine->SetMaxFPS(60);

	Super::BeginPlay();

	AMousePlayerController* PlayerController = Cast<AMousePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController) {
		PlayerController->OnDoubleClick.AddUObject(this, &ATableGameMode::ManageDoubleClick);
		PlayerController->OnReleased.AddUObject(this, &ATableGameMode::ManageRelease);
	}

	Spawn(FVector::ZeroVector);
}

void ATableGameMode::OnActorLoaded() {
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, TEXT("Table class asynchronously loaded"));
	ConsumePendingSpawns();
}

void ATableGameMode::ConsumePendingSpawns() {
	if (m_ActorToSpawn.Get()) {
		const TArray<FVector> positions = m_PendingSpawnPoints;
		m_PendingSpawnPoints.Reset();
		for (const FVector& position : positions) {
			Spawn(position);
		}
	}
}

void ATableGameMode::ManageDoubleClick(const FVector& Position, AActor* Actor, FKey Key) {
	if (Key == EKeys::LeftMouseButton) {
		Spawn(Position);
	} else if (Key == EKeys::RightMouseButton) {
		if (m_SpawnedActors.Find(Actor) != INDEX_NONE) {
			Delete(Actor);
		}
	}
}

void ATableGameMode::ManageRelease(const FVector& Position, AActor* Actor, FKey Key) {
	BroadcastMouseReleased(Actor, Key);
}

void ATableGameMode::Spawn(const FVector& SpawnPoint) {
	if (!m_ActorToSpawn.IsNull()) {
		if (!m_ActorToSpawn.Get()) {
			UAsyncFunctionLibrary::RequestAsyncLoad(m_ActorToSpawn, FStreamableDelegate::CreateUObject(this, &ATableGameMode::OnActorLoaded));
			m_PendingSpawnPoints.Push(SpawnPoint);
		} else {
			AActor* ActorToSpawn;
			if (m_ActorPool.Num()) {
				ActorToSpawn = m_ActorPool.Pop();
				ActorToSpawn->SetActorHiddenInGame(false);
				ActorToSpawn->SetActorEnableCollision(true);
				ActorToSpawn->SetActorLocation(SpawnPoint);
				GEngine->AddOnScreenDebugMessage(2, 2, FColor::Green, TEXT("Fetched actor from pool"));
			} else {
				ActorToSpawn = GetWorld()->SpawnActor(m_ActorToSpawn.Get(), &SpawnPoint);
				GEngine->AddOnScreenDebugMessage(2, 2, FColor::Green, TEXT("Created a new actor"));
			}
			m_SpawnedActors.Push(ActorToSpawn);
		}
	}
}

void ATableGameMode::BroadcastMouseReleased(AActor* ReleasedActor, FKey Key) const {
	if (ReleasedActor) {
		ReleasedActor->OnReleased.Broadcast(ReleasedActor, Key);
	}
}

void ATableGameMode::Delete(AActor* ActorToDelete) {
	int32 Index = m_SpawnedActors.Find(ActorToDelete);
	AActor* Actor = m_SpawnedActors[Index];
	m_SpawnedActors.RemoveAt(Index);

	if (m_ActorPool.Num() < m_PoolSize) {
		ActorToDelete->SetActorEnableCollision(false);
		ActorToDelete->SetActorHiddenInGame(true);
		ActorToDelete->Reset();
		m_ActorPool.Push(Actor);
		GEngine->AddOnScreenDebugMessage(3, 2, FColor::Green, TEXT("Table disappeared into pool"));
	} else {
		Actor->Destroy();
		GEngine->AddOnScreenDebugMessage(3, 2, FColor::Green, TEXT("Table was destroyed"));
	}
}
