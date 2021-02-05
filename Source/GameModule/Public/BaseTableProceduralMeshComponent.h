// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "BaseTableProceduralMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMEMODULE_API UBaseTableProceduralMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	// Generate the procedural mesh
	virtual void Generate(const TArray<FVector>& Vertices) PURE_VIRTUAL(, );
	// Update the procedural mesh
	virtual void Update(const TArray<FVector>& Vertices) PURE_VIRTUAL(, );

	const float& GetThickness() const { return m_Thickness; };
	
	UPROPERTY(EditAnywhere, DisplayName = "Material")
	UMaterial* m_Material;
	UPROPERTY(EditAnywhere, DisplayName = "Legs Material")
	UMaterial* m_LegsMaterial;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1"), DisplayName = "Thickness")
	float m_Thickness;

	// Maintain the current mesh vertices to correctly update the component
	TArray<FVector> m_Vertices;
};
