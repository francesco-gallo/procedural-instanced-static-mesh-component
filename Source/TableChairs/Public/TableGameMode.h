// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TableGameMode.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class TABLECHAIRS_API ATableGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATableGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Actor to Spawn")
	TSoftClassPtr<AActor> m_ActorToSpawn;

	TArray<FVector> m_PendingSpawnPoints;

	UPROPERTY(VisibleInstanceOnly, DisplayName = "Spawned Actors")
	TArray<AActor*> m_SpawnedActors;

protected:

private:
	void OnActorLoaded();
	// In case there have been spawn request before the loading of the actor
	void ConsumePendingSpawns();
	void ManageDoubleClick(const FVector& Position, AActor* Actor, FKey Key);
	void ManageRelease(const FVector& Position, AActor* Actor, FKey Key);
	void Spawn(const FVector& Position);
	void BroadcastMouseReleased(AActor* ActorReleased, FKey Key) const;
	void Delete(AActor* ActorToDelete);

	// Why create when you can recycle?
	UPROPERTY(VisibleInstanceOnly, DisplayName = "Actors Pool")
	TArray<AActor*> m_ActorPool;
	// A pool size is to avoid uselessly used memory
	int32 m_PoolSize;
};