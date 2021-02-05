// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "BaseChairProceduralMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMEMODULE_API UBaseChairProceduralMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	// Create the procedural mesh
	virtual void Generate() PURE_VIRTUAL(, );

	const float& GetSide() const { return m_Side; };
	const float& GetHeight() const { return m_Height; };
	const float& GetInitialRotation() const { return m_InitialRotation; };
	
	UPROPERTY(EditDefaultsOnly, DisplayName = "Material")
	UMaterial* m_Material;

protected:

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0.1"), DisplayName = "Side")
	float m_Side;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1"), DisplayName = "Height")
	float m_Height;

	// Correct the mesh initial orientation
	UPROPERTY(EditDefaultsOnly, DisplayName = "Chair Initial Rotation")
	float m_InitialRotation;
};
