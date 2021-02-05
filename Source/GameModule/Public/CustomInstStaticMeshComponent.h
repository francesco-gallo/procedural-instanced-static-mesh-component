// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "CustomInstStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Rendering, meta = (BlueprintSpawnableComponent), Blueprintable)
class GAMEMODULE_API UCustomInstStaticMeshComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	// There is no batch remove instances on UInstancedStaticMeshComponent, and calling RemoveInstance 700 times has its toll...
	void PopInstances(int32 Count);
	
};
