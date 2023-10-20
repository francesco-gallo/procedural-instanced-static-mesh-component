// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomInstStaticMeshComponent.h"
#include <AI/NavigationSystemBase.h>

void UCustomInstStaticMeshComponent::PopInstances(int32 Count) {
	if (Count > 0) {
		// Request navigation update
		FNavigationSystem::UpdateComponentData(*this);

		const int32 FinalIndex = PerInstanceSMData.Num() - Count;
		for (int32 InstanceIndex = PerInstanceSMData.Num() - 1; InstanceIndex >= FinalIndex; --InstanceIndex) {
			// remove instance
			if (PerInstanceSMData.IsValidIndex(InstanceIndex)) {
				PerInstanceSMData.RemoveAt(InstanceIndex);
				PerInstanceSMCustomData.RemoveAt(InstanceIndex * NumCustomDataFloats, NumCustomDataFloats);
			}

#if WITH_EDITOR
			// remove selection flag if array is filled in
			if (SelectedInstances.IsValidIndex(InstanceIndex)) {
				SelectedInstances.RemoveAt(InstanceIndex);
			}
#endif

			// update the physics state
			if (bPhysicsStateCreated && InstanceBodies.IsValidIndex(InstanceIndex)) {
				if (FBodyInstance*& InstanceBody = InstanceBodies[InstanceIndex]) {
					InstanceBody->TermBody();
					delete InstanceBody;
					InstanceBody = nullptr;

					InstanceBodies.RemoveAt(InstanceIndex);

					// Re-target instance indices for shifting of array.
					for (int32 i = InstanceIndex; i < InstanceBodies.Num(); ++i) {
						InstanceBodies[i]->InstanceBodyIndex = i;
					}
				}
			}
		}

		// Force recreation of the render data
		const int32 a = InstanceUpdateCmdBuffer.NumTotalCommands();
		MarkRenderStateDirty();
	}
}
