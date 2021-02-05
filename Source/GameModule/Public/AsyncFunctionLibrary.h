// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "AsyncFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMODULE_API UAsyncFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Asynchronously load a SoftClassPtr and calls the delegate when ready; Insta-calls if the object is already live.
	template< typename T >
	static void RequestAsyncLoad(TSoftClassPtr<T>& SoftClassPtr, FStreamableDelegate Delegate);
};

template< typename T >
void UAsyncFunctionLibrary::RequestAsyncLoad(TSoftClassPtr<T>& SoftClassPtr, FStreamableDelegate Delegate) {
	if (!SoftClassPtr.IsNull()) {
		if (!SoftClassPtr.Get()) {
			UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftClassPtr.ToSoftObjectPath(), Delegate);
		} else {
			Delegate.ExecuteIfBound();
		}
	}
}
