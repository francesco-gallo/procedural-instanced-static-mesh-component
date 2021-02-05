// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizableTable.h"
#include "Kismet/GameplayStatics.h"
#include "BaseTableProceduralMeshComponent.h"
#include "BaseChairProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MeshFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "CustomInstStaticMeshComponent.h"

AResizableTable::AResizableTable() {
	PrimaryActorTick.bCanEverTick = true;

	m_MinimumLength = 100.f;
	m_TableHeight = 50.f;
	m_MinChairDistance = 10.f;
	m_HandlerScale = FVector::OneVector;
	m_OldChairsCount = 0;
	m_UseFastCollision = FakeChairCollision::AUTO;
	m_FastCollision = false;
	m_SmoothPlacement = true;
}

void AResizableTable::BeginPlay() {
	Super::BeginPlay();

	ConditionalSetupFakeCollision();
	ResetVertices();
	CreateHandlers();
	CreateProceduralMesh();
	CreateInstancedChair();
	UpdateChairs();
}

void AResizableTable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	ConditionalHandleDrag();
}

void AResizableTable::Reset() {
	Super::Reset();
	ResetVertices();
	for (int32 i = 0; i < m_Handlers.Num() && i < m_RelativeVertices.Num(); ++i) {
		m_Handlers[i]->SetRelativeLocation(m_RelativeVertices[i]);
	}
	UpdateChairs();
}

void AResizableTable::CreateHandlers() {
	if (m_HandlerMeshClass) {
		for (int32 i = 0; i < m_RelativeVertices.Num(); ++i) {
			UStaticMeshComponent* Handler = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
			Handler->SetStaticMesh(m_HandlerMeshClass);
			Handler->SetRelativeScale3D(m_HandlerScale);
			if (m_HandlerMaterial) {
				Handler->CreateAndSetMaterialInstanceDynamicFromMaterial(0, m_HandlerMaterial);
			}

			Handler->RegisterComponent();
			AddInstanceComponent(Handler);
			Handler->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Handler->SetRelativeLocation(m_RelativeVertices[i]);
			Handler->OnClicked.AddUniqueDynamic(this, &AResizableTable::AngleClicked);
			m_Handlers.Add(Handler);
		}
	}

	OnReleased.AddUniqueDynamic(this, &AResizableTable::MouseReleased);
}

void AResizableTable::ResetVertices() {
	const int32 HalfLength = m_MinimumLength / 2;
	m_RelativeVertices.Reset(4);
	m_RelativeVertices.Push(FVector(-HalfLength, -HalfLength, m_TableHeight));
	m_RelativeVertices.Push(FVector(-HalfLength, HalfLength, m_TableHeight));
	m_RelativeVertices.Push(FVector(HalfLength, HalfLength, m_TableHeight));
	m_RelativeVertices.Push(FVector(HalfLength, -HalfLength, m_TableHeight));
	UpdateProceduralMesh();
}

void AResizableTable::CreateProceduralMesh() {
	if (m_ProceduralTableClass) {
		m_ProceduralMesh = NewObject<UBaseTableProceduralMeshComponent>(this, m_ProceduralTableClass);
		m_ProceduralMesh->RegisterComponent();
		m_ProceduralMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		AddInstanceComponent(m_ProceduralMesh);
		m_ProceduralMesh->Generate(m_RelativeVertices);
	}
}

void AResizableTable::UpdateProceduralMesh() {
	if (m_ProceduralMesh) {
		m_ProceduralMesh->Update(m_RelativeVertices);
	}
}

void AResizableTable::AngleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) {
	if (ButtonPressed == EKeys::LeftMouseButton) {
		m_DraggingHandler = TouchedComponent;
		for (UPrimitiveComponent* Handler : m_Handlers) {
			if (Handler != m_DraggingHandler) {
				if (UMaterialInstanceDynamic* Material = (UMaterialInstanceDynamic*)Handler->GetMaterial(0)) {
					Material->SetScalarParameterValue("Opacity", 0.2);
				}
			}
		}
	}
}

void AResizableTable::MouseReleased(AActor* Actor, FKey ButtonPressed) {
	if (ButtonPressed == EKeys::LeftMouseButton && m_DraggingHandler) {
		for (UPrimitiveComponent* Handler : m_Handlers) {
			if (Handler != m_DraggingHandler) {
				if (UMaterialInstanceDynamic* Material = (UMaterialInstanceDynamic*)Handler->GetMaterial(0)) {
					Material->SetScalarParameterValue("Opacity", 0.5);
				}
			}
		}
		m_DraggingHandler = nullptr;
	}
}

void AResizableTable::ConditionalHandleDrag() {
	if (m_DraggingHandler) {
		const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController) {
			FVector StartTrace;
			FRotator dummy;
			PlayerController->GetPlayerViewPoint(StartTrace, dummy);

			FVector MouseLocation, MouseDirection;
			if (PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection)) {
				const FVector& InitialLocation = m_DraggingHandler->GetComponentLocation();
				const FVector& EndTrace = MouseLocation + (MouseDirection * 10000);
				// To avoid negative mouse placement
				if (MouseLocation.Z > EndTrace.Z) {
					// to keep the handler in its horizontal plane
					const FVector& Intersection = FMath::LinePlaneIntersection(StartTrace, EndTrace, InitialLocation, FVector(0, 0, 1));
					if (UpdateHandlesPosition(m_DraggingHandler, Intersection)) {
						UpdateChairs();
					}
				}
			}
		}
	}
}

bool AResizableTable::UpdateHandlesPosition(UPrimitiveComponent* Handle, const FVector& NewPosition) {
	if (Handle) {
		const int32 HandleIndex = m_Handlers.Find(Handle);

		float OppositeIndex = (HandleIndex + 2) % 4;
		const UPrimitiveComponent* OppositeHandle = m_Handlers[OppositeIndex];
		const FVector& OppositeLocation = OppositeHandle->GetComponentLocation();

		FVector CorrectPosition(NewPosition);
		// Keep the handle in its table quarter, bound by the minimum table length
		switch (HandleIndex) {
			case(0): {
				CorrectPosition.X = FMath::Clamp(NewPosition.X, NewPosition.X, OppositeLocation.X - m_MinimumLength);
				CorrectPosition.Y = FMath::Clamp(NewPosition.Y, NewPosition.Y, OppositeLocation.Y - m_MinimumLength);
				break;
			}
			case(1): {
				CorrectPosition.X = FMath::Clamp(NewPosition.X, NewPosition.X, OppositeLocation.X - m_MinimumLength);
				CorrectPosition.Y = FMath::Clamp(NewPosition.Y, OppositeLocation.Y + m_MinimumLength, NewPosition.Y);
				break;
			}
			case(2): {
				CorrectPosition.X = FMath::Clamp(NewPosition.X, OppositeLocation.X + m_MinimumLength, NewPosition.X);
				CorrectPosition.Y = FMath::Clamp(NewPosition.Y, OppositeLocation.Y + m_MinimumLength, NewPosition.Y);
				break;
			}
			case(3): {
				CorrectPosition.X = FMath::Clamp(NewPosition.X, OppositeLocation.X + m_MinimumLength, NewPosition.X );
				CorrectPosition.Y = FMath::Clamp(NewPosition.Y, NewPosition.Y , OppositeLocation.Y - m_MinimumLength);
				break;
			} 
		}

		const FVector& InitialLocation = Handle->GetComponentLocation();
		if (InitialLocation == CorrectPosition) {
			GEngine->AddOnScreenDebugMessage(6, 1, FColor::Orange, TEXT("Handle didn't move; skipping update"));
			return false;
		}

		if (m_SmoothPlacement) {
			if (FVector::Distance(InitialLocation, OppositeLocation) < FVector::Distance(CorrectPosition, OppositeLocation)) {
				GEngine->AddOnScreenDebugMessage(5, 1, FColor::Orange, TEXT("Limiting handle position"));
				CorrectPosition = (CorrectPosition - InitialLocation).GetClampedToMaxSize2D(m_FastCollision ? 1000000 : 10000) + InitialLocation;
			}
		}

		Handle->SetWorldLocation(CorrectPosition);

		float PrevIndex = (HandleIndex + 3) % 4;
		float NextIndex = (HandleIndex + 1) % 4;
		// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("%u, %f, %f, %s"), HandleIndex, PrevIndex, NextIndex, *CorrectPosition.ToString()));
		UPrimitiveComponent* PrevHandle = m_Handlers[PrevIndex];
		UPrimitiveComponent* NextHandle = m_Handlers[NextIndex];
		FVector Prev = PrevHandle->GetComponentLocation();
		FVector Next = NextHandle->GetComponentLocation();
		if (HandleIndex % 2) {
			Next.Y = CorrectPosition.Y;
			Prev.X = CorrectPosition.X;
		} else {
			Next.X = CorrectPosition.X;
			Prev.Y = CorrectPosition.Y;
		}
		NextHandle->SetWorldLocation(Next);
		PrevHandle->SetWorldLocation(Prev);
		
		const FTransform& WorldTransform = GetTransform();
		m_RelativeVertices[HandleIndex] = WorldTransform.InverseTransformPosition(CorrectPosition);
		m_RelativeVertices[NextIndex] = WorldTransform.InverseTransformPosition(Next);
		m_RelativeVertices[PrevIndex] = WorldTransform.InverseTransformPosition(Prev);

		UpdateProceduralMesh();

		return true;
	}
	return false;
}

void AResizableTable::CreateInstancedChair() {
	if (m_ProceduralChairClass) {
		UBaseChairProceduralMeshComponent* Chair = NewObject<UBaseChairProceduralMeshComponent>(this, m_ProceduralChairClass);
		Chair->Generate();
		UStaticMesh* Mesh = UMeshFunctionLibrary::GetMeshFromProceduralMesh(Chair);
		Chair->DestroyComponent();
		m_InstancedChairComponent = NewObject<UCustomInstStaticMeshComponent>(this, UCustomInstStaticMeshComponent::StaticClass());
		m_InstancedChairComponent->SetStaticMesh(Mesh);
		m_InstancedChairComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		if (m_FastCollision) {
			m_InstancedChairComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		} else {
			m_InstancedChairComponent->OnComponentCollisionSettingsChangedEvent.AddUniqueDynamic(this, &AResizableTable::RefreshChairsPhysicState);
		}
		m_InstancedChairComponent->RegisterComponent();
		AddInstanceComponent(m_InstancedChairComponent);
	}
}

void AResizableTable::UpdateChairs() {
	if (m_InstancedChairComponent && m_ProceduralChairClass) {
		const float& TableLegSide = m_ProceduralMesh ? m_ProceduralMesh->GetThickness() : 0.f;

		const float& SpaceX = FVector::Distance(m_RelativeVertices[0], m_RelativeVertices[1]) - (TableLegSide * 2);
		const float& SpaceY = FVector::Distance(m_RelativeVertices[1], m_RelativeVertices[2]) - (TableLegSide * 2);

		const UBaseChairProceduralMeshComponent* DefaultChairObject = m_ProceduralChairClass->GetDefaultObject<UBaseChairProceduralMeshComponent>();
		const float& ChairSide = DefaultChairObject->GetSide();
		const float& InitialRotation = DefaultChairObject->GetInitialRotation();

		const float ChairSpace = ChairSide + m_MinChairDistance; //Half on left, half on right side

		const int32 ChairsX = SpaceX / ChairSpace;
		const int32 ChairsY = SpaceY / ChairSpace;

		const int32 NewChairsCount = (ChairsX + ChairsY) * 2;

		const FVector& NearestHandleLoc = m_RelativeVertices[0];
		const FVector& XFurtherHandleLoc = m_RelativeVertices[3];

		// Rotator logic is to support eventual table rotation
		FRotator XRot = (XFurtherHandleLoc - NearestHandleLoc).Rotation();
		const FTransform XNearestHandleTra(XRot, NearestHandleLoc);
		const FTransform XFurtherHandleTra(XRot, XFurtherHandleLoc);

		const float RemainingXSpace = (SpaceX - ChairsX * ChairSpace) / (ChairsX + 1); // remaining space that can be added to each chair

		TArray<FTransform> UpdatedTransforms;
		UpdatedTransforms.AddUninitialized(NewChairsCount);

		int32 Count = 0;
		for (int32 i = 0; i < ChairsX; ++i) {
			const FVector DeltaLoc(0, ChairSide / 2.f + TableLegSide + m_MinChairDistance / 2 + ChairSpace * i + (RemainingXSpace * (i + 1)), -m_TableHeight);	// ChairSide / 2 because the chair's pivot is in its middle so we are offsetting that.
			UpdatedTransforms[Count++] = FTransform(XNearestHandleTra.Rotator() + FRotator(0, InitialRotation, 0), NearestHandleLoc + DeltaLoc);
			UpdatedTransforms[Count++] = FTransform(XFurtherHandleTra.Rotator() + FRotator(0, InitialRotation + 180.f, 0), XFurtherHandleLoc + DeltaLoc);
		}

		const FVector& YFurtherHandleLoc = m_RelativeVertices[1];
		FRotator YRot = (YFurtherHandleLoc - NearestHandleLoc).Rotation();
		const FTransform YNearestHandleTra(YRot, NearestHandleLoc);
		const FTransform YFurtherHandleTra(YRot, YFurtherHandleLoc);

		const float RemainingYSpace = (SpaceY - ChairsY * ChairSpace) / (ChairsY + 1);

		for (int32 i = 0; i < ChairsY; ++i) {
			const FVector DeltaLoc(ChairSide / 2.f + TableLegSide + m_MinChairDistance / 2 + ChairSpace * i + (RemainingYSpace * (i + 1)), 0, -m_TableHeight);
			UpdatedTransforms[Count++] = FTransform(YNearestHandleTra.Rotator() + FRotator(0, InitialRotation, 0), NearestHandleLoc + DeltaLoc);
			UpdatedTransforms[Count++] = FTransform(YFurtherHandleTra.Rotator() + FRotator(0, InitialRotation + 180.f, 0), YFurtherHandleLoc + DeltaLoc);
		}

		const int32 DeltaChairs = NewChairsCount - m_OldChairsCount;
		if (DeltaChairs > 0) {	// Need more chairs
			TArray<FTransform> NewTransforms;
			NewTransforms.Reserve(DeltaChairs);
			for (int32 i = 0; i < DeltaChairs; ++i) {
				NewTransforms.Push(UpdatedTransforms.Pop());
			}
			AddChairs(NewTransforms);
		} else if (DeltaChairs < 0) {	// Too many chairs
			RemoveChairs(-DeltaChairs);
		}

		// We are manually updating some chairs
		if (UpdatedTransforms.Num()) {
			m_InstancedChairComponent->BatchUpdateInstancesTransforms(0, UpdatedTransforms, false, false, true);
			m_InstancedChairComponent->MarkRenderStateDirty();	// faster than setting only the last chair dirty
		}

		if (m_FastCollision) {
			const float& CollisionHeight = DefaultChairObject->GetHeight() / 2;
			const FVector Middle0((m_RelativeVertices[0] + m_RelativeVertices[1]) / 2);
			const FVector Middle1((m_RelativeVertices[1] + m_RelativeVertices[2]) / 2);
			const FVector Middle2((m_RelativeVertices[2] + m_RelativeVertices[3]) / 2);
			const FVector Middle3((m_RelativeVertices[3] + m_RelativeVertices[0]) / 2);

			m_BoxComponents[0]->SetRelativeLocation(FVector(Middle0.X, Middle0.Y, CollisionHeight));
			m_BoxComponents[1]->SetRelativeLocation(FVector(Middle1.X, Middle1.Y, CollisionHeight));
			m_BoxComponents[2]->SetRelativeLocation(FVector(Middle2.X, Middle2.Y, CollisionHeight));
			m_BoxComponents[3]->SetRelativeLocation(FVector(Middle3.X, Middle3.Y, CollisionHeight));
			m_BoxComponents[0]->SetBoxExtent(FVector(ChairSide / 2, SpaceX / 2, CollisionHeight));
			m_BoxComponents[1]->SetBoxExtent(FVector(ChairSide / 2, SpaceY / 2, CollisionHeight));
			m_BoxComponents[2]->SetBoxExtent(FVector(ChairSide / 2, SpaceX / 2, CollisionHeight));
			m_BoxComponents[3]->SetBoxExtent(FVector(ChairSide / 2, SpaceY / 2, CollisionHeight));
		}

		m_OldChairsCount = NewChairsCount;

		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Green, FString::Printf(TEXT("This table has now %u chairs"), m_OldChairsCount));
	}
}

void AResizableTable::AddChairs(const TArray<FTransform>& NewTransforms) {
	if (m_InstancedChairComponent) {
		m_InstancedChairComponent->PreAllocateInstancesMemory(NewTransforms.Num());
		m_InstancedChairComponent->AddInstances(NewTransforms, false);
	}
}

void AResizableTable::RemoveChairs(int32 Count) {
	if (m_InstancedChairComponent) {
		m_InstancedChairComponent->PopInstances(Count);
	}
}

void AResizableTable::RefreshChairsPhysicState(UPrimitiveComponent* ChangedComponent) {
	if (m_InstancedChairComponent->IsCollisionEnabled()) {
		m_InstancedChairComponent->CreatePhysicsState();
		m_InstancedChairComponent->MarkRenderStateDirty();
	} else {
		m_InstancedChairComponent->DestroyPhysicsState();
	}
}

void AResizableTable::ConditionalSetupFakeCollision() {
	if (UseFastChairCollisions(m_UseFastCollision)) {
		GEngine->AddOnScreenDebugMessage(4, 5, FColor::Green, TEXT("Fast chair collision is enabled"));
		m_FastCollision = true;
		for (int32 i = 0; i < 4; ++i) {
			UBoxComponent* CollisionComponent = NewObject<UBoxComponent>(this);
			CollisionComponent->RegisterComponent();
			CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			CollisionComponent->SetCollisionProfileName(TEXT("InvisibleWall"));
			AddInstanceComponent(CollisionComponent);
			CollisionComponent->SetRelativeRotation(FRotator(0, 90 * i, 0));
			m_BoxComponents.Push(CollisionComponent);
		}
	}
}

bool AResizableTable::UseFastChairCollisions(const FakeChairCollision& CollisionEnum) const {
	switch (CollisionEnum) {
		case FakeChairCollision::NEVER: {
			return false;
		}
		case FakeChairCollision::ALWAYS: {
			return true;
		}
		case FakeChairCollision::AUTO:
			if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
				FVector dummy, HalfExtent;
				PlayerController->GetPawn()->GetActorBounds(false, dummy, HalfExtent);
				float PawnWidth = HalfExtent.Y * 2;
				return (PawnWidth > m_MinChairDistance); // If pawn size is more than chair offset, we don't really need to compute each chair physic body
			}
		default: {
			return false;
		}
	}
}
