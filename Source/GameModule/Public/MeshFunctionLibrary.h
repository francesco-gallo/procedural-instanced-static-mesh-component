// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshFunctionLibrary.generated.h"

/**
 * 
 */

class UProceduralMeshComponent;
class UStaticMesh;

UCLASS()
class GAMEMODULE_API UMeshFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
public:

	// Generate a usable static mesh from a procedural mesh component
	static UStaticMesh* GetMeshFromProceduralMesh(UProceduralMeshComponent* ProceduralMesh);
	
};
