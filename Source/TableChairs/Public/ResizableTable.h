// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResizableTable.generated.h"

class UStaticMeshComponent;
class UBaseTableProceduralMeshComponent;
class UBaseChairProceduralMeshComponent;
class UBoxComponent;
class UCustomInstStaticMeshComponent;

UENUM(BlueprintType)
enum class FakeChairCollision : uint8 {
	AUTO = 0 UMETA(DisplayName = "Depends on pawn width"),
	NEVER UMETA(DisplayName = "Never"),
	ALWAYS UMETA(DisplayName = "Always"),
};

UCLASS()
class TABLECHAIRS_API AResizableTable : public AActor
{
	GENERATED_BODY()
	
public:	
	AResizableTable();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Reset() override;

protected:
	UPROPERTY(BlueprintReadOnly, DisplayName = "Vertices")
	TArray<FVector> m_RelativeVertices;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Handler Mesh")
	UStaticMesh* m_HandlerMeshClass;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Handler Scale")
	FVector m_HandlerScale;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Handler Material")
	UMaterial* m_HandlerMaterial;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Table Mesh")
	TSubclassOf<UBaseTableProceduralMeshComponent> m_ProceduralTableClass;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Chair Mesh")
	TSubclassOf<UBaseChairProceduralMeshComponent> m_ProceduralChairClass;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Minimum Length", meta=(ClampMin = "0.1"))
	float m_MinimumLength;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Table Height", meta=(ClampMin = "0.1"))
	float m_TableHeight;

	UPROPERTY(EditAnywhere, DisplayName = "Minimum Chair Distance", meta=(ClampMin="0"))
	float m_MinChairDistance;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Use Fast Physics")
	FakeChairCollision m_UseFastCollision;

	UPROPERTY(EditAnywhere, DisplayName = "Smooth Chairs Placement")
	bool m_SmoothPlacement;

private:
	// Keep handlers reference to edit their material
	UPROPERTY()
	TArray<UPrimitiveComponent*> m_Handlers;
	// Currently dragged handler. Kept to conditionally update the table
	UPrimitiveComponent* m_DraggingHandler;

	// Procedural table mesh component, added at runtime.
	UPROPERTY()
	UBaseTableProceduralMeshComponent* m_ProceduralMesh;

	// Chairs instanced mesh component, added at runtime.
	UPROPERTY()
	UCustomInstStaticMeshComponent* m_InstancedChairComponent;

	// Used to conditionally fake chairs collision bounds
	UPROPERTY()
	TArray<UBoxComponent*> m_BoxComponents;

	// Kept to update chairs only when needed
	int32 m_OldChairsCount;

	bool m_FastCollision;

	void CreateHandlers();

	// Restore initial table state
	void ResetVertices();

	void CreateProceduralMesh();
	void UpdateProceduralMesh();

	UFUNCTION()
	void AngleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void MouseReleased(AActor* Actor, FKey ButtonPressed);

	// Checks whether to update handlers and chairs
	void ConditionalHandleDrag();
	bool UpdateHandlesPosition(UPrimitiveComponent* Handle, const FVector& NewPosition);

	// Generates the instanced mesh component from the procedural mesh
	void CreateInstancedChair();
	// Calculate and place/remove new chairs
	void UpdateChairs();
	// Create new chairs, with their correct initial position to avoid repositioning them
	void AddChairs(const TArray<FTransform>& NewTransforms);
	void RemoveChairs(int32 Count);

	// Chairs don't reflect the parent's collision changes...
	UFUNCTION()
	void RefreshChairsPhysicState(UPrimitiveComponent* ChangedComponent);
	// Setups 4 fake collisions to simulate the blocking chairs
	void ConditionalSetupFakeCollision();
	// Checks whether to use the fake approach or the standard system
	bool UseFastChairCollisions(const FakeChairCollision& CollisionEnum) const;
};
