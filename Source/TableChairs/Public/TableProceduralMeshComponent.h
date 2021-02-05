// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTableProceduralMeshComponent.h"
#include "TableProceduralMeshComponent.generated.h"

/**
 *   3-----------2
 *   |           |
 *   |           |
 *   |           |
 *   0-----------1
 * 
 * 
 *   7--31---24--6
 *   29-30   22-23
 *   |           |
 *   10--9   17-16
 *   4---8---15--5 
 * 
 * 
 *   35-34   28-27
 *   32-33   25-26
 *
 *   14-13   21-20
 *   11-12   18-19
 * 
 */
UCLASS()
class TABLECHAIRS_API UTableProceduralMeshComponent : public UBaseTableProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UTableProceduralMeshComponent();

	virtual void Generate(const TArray<FVector>& Vertices) override;
	virtual void Update(const TArray<FVector>& Vertices) override;

private:
	void UpdateVertices(const TArray<FVector>& Vertices);

	TArray<FVector2D> m_UV;
	TArray<FVector2D> m_Side0UV;
	TArray<FVector2D> m_Side1UV;
	TArray<FVector2D> m_Side2UV;
	TArray<FVector2D> m_Side3UV;

	// Used to compute UV delta
	float m_origDX;
	// Used to compute UV delta
	float m_origDY;

	static int32 s_verticesCount;
};
