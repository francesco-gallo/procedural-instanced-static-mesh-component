// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshFunctionLibrary.h"
#include "MeshDescription.h"
#include "ProceduralMeshConversion.h"
#include "ProceduralMeshComponent.h"

UStaticMesh* UMeshFunctionLibrary::GetMeshFromProceduralMesh(UProceduralMeshComponent* ProceduralMesh) {
	const FMeshDescription& MeshDescription = BuildMeshDescription(ProceduralMesh);

	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(ProceduralMesh->GetOwner(), UStaticMesh::StaticClass());

	StaticMesh->CreateBodySetup();

	if (!ProceduralMesh->bUseComplexAsSimpleCollision) {
		UBodySetup* NewBodySetup = StaticMesh->BodySetup;
		NewBodySetup->BodySetupGuid = FGuid::NewGuid();
		NewBodySetup->AggGeom.ConvexElems = ProceduralMesh->ProcMeshBodySetup->AggGeom.ConvexElems;
		NewBodySetup->bGenerateMirroredCollision = false;
		NewBodySetup->bDoubleSidedGeometry = true;
		NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
		NewBodySetup->CreatePhysicsMeshes();
	}

	TSet<UMaterialInterface*> UniqueMaterials;
	const int32 NumSections = ProceduralMesh->GetNumSections();
	for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++) {
		FProcMeshSection* ProcSection = ProceduralMesh->GetProcMeshSection(SectionIdx);
		UMaterialInterface* Material = ProceduralMesh->GetMaterial(SectionIdx);
		UniqueMaterials.Add(Material);
	}

	for (auto* Material : UniqueMaterials) {
		StaticMesh->StaticMaterials.Add(FStaticMaterial(Material));
	}

	UStaticMesh::FBuildMeshDescriptionsParams BuildParams;
	BuildParams.bBuildSimpleCollision = true;
	StaticMesh->BuildFromMeshDescriptions({ &MeshDescription }, BuildParams);

	return StaticMesh;
}
