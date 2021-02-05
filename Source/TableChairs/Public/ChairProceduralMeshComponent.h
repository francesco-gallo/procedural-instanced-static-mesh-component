// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChairProceduralMeshComponent.h"
#include "ChairProceduralMeshComponent.generated.h"

/**
 *   3-2
 *   | |
 *   | |
 *   | |
 *   0-1
 * 
 * 
 *   7-6--------9
 *   | |        |
 *   | |        |
 *   | |        |
 *   4-5--------8
 * 
 *        
 *   13-37--30-12
 *   35-36  28-29
 *   |          |
 *   16-15  23-22
 *   10-14--21-11
 * 
 * 
 *   41-40  34-33
 *   38-39  31-32
 *
 *   20-19  27-26
 *   17-18  24-25
 *
 */
UCLASS()
class TABLECHAIRS_API UChairProceduralMeshComponent : public UBaseChairProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UChairProceduralMeshComponent();

	virtual void Generate() override;

protected:
	UPROPERTY(EditDefaultsOnly, DisplayName = "Thickness")
	float m_Thickness;

private:
	static int32 s_verticesCount;
};
