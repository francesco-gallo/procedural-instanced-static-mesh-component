// Fill out your copyright notice in the Description page of Project Settings.


#include "ChairProceduralMeshComponent.h"

int32 UChairProceduralMeshComponent::s_verticesCount = 42;

UChairProceduralMeshComponent::UChairProceduralMeshComponent() {
	m_Thickness = 5.f;
	m_Side = 30;
	m_Height = 80;
	m_InitialRotation = -90.f;
}

void UChairProceduralMeshComponent::Generate() {
	TArray<FVector> Vertices;
	Vertices.SetNumUninitialized(s_verticesCount);
	int32 HalfSide = m_Side / 2;
	int32 LegsHeight = m_Height / 2 - m_Thickness;

	Vertices[0] = FVector(-HalfSide, -HalfSide - m_Thickness, m_Height);
	Vertices[1] = Vertices[0] + FVector(0, m_Thickness, 0);
	Vertices[2] = Vertices[0] + FVector(m_Side, m_Thickness, 0);
	Vertices[3] = Vertices[0] + FVector(m_Side, 0, 0);

	Vertices[4] = Vertices[0] - FVector(0, 0, m_Height / 2);
	Vertices[5] = Vertices[1] - FVector(0, 0, m_Height / 2);
	Vertices[6] = Vertices[2] - FVector(0, 0, m_Height / 2);
	Vertices[7] = Vertices[3] - FVector(0, 0, m_Height / 2);

	Vertices[8] = Vertices[5] + FVector(0, m_Side, 0);
	Vertices[9] = Vertices[6] + FVector(0, m_Side, 0);
	Vertices[10] = Vertices[4] - FVector(0, 0, m_Thickness);
	Vertices[11] = Vertices[8] - FVector(0, 0, m_Thickness);
	Vertices[12] = Vertices[9] - FVector(0, 0, m_Thickness);
	Vertices[13] = Vertices[7] - FVector(0, 0, m_Thickness);

	Vertices[14] = Vertices[10] + FVector(0, m_Thickness, 0);
	Vertices[15] = Vertices[10] + FVector(m_Thickness, m_Thickness, 0);
	Vertices[16] = Vertices[10] + FVector(m_Thickness, 0, 0);
	Vertices[17] = Vertices[10] - FVector(0, 0, LegsHeight);
	Vertices[18] = Vertices[14] - FVector(0, 0, LegsHeight);
	Vertices[19] = Vertices[15] - FVector(0, 0, LegsHeight);
	Vertices[20] = Vertices[16] - FVector(0, 0, LegsHeight);

	Vertices[21] = Vertices[11] + FVector(0, -m_Thickness, 0);
	Vertices[22] = Vertices[11] + FVector(m_Thickness, 0, 0);
	Vertices[23] = Vertices[11] + FVector(m_Thickness, -m_Thickness, 0);
	Vertices[24] = Vertices[21] - FVector(0, 0, LegsHeight);
	Vertices[25] = Vertices[11] - FVector(0, 0, LegsHeight);
	Vertices[26] = Vertices[22] - FVector(0, 0, LegsHeight);
	Vertices[27] = Vertices[23] - FVector(0, 0, LegsHeight);

	Vertices[28] = Vertices[12] + FVector(-m_Thickness, -m_Thickness, 0);
	Vertices[29] = Vertices[12] + FVector(-m_Thickness, 0, 0);
	Vertices[30] = Vertices[12] + FVector(0, -m_Thickness, 0);
	Vertices[31] = Vertices[28] - FVector(0, 0, LegsHeight);
	Vertices[32] = Vertices[29] - FVector(0, 0, LegsHeight);
	Vertices[33] = Vertices[12] - FVector(0, 0, LegsHeight);
	Vertices[34] = Vertices[30] - FVector(0, 0, LegsHeight);

	Vertices[35] = Vertices[13] + FVector(-m_Thickness, 0, 0);
	Vertices[36] = Vertices[13] + FVector(-m_Thickness, m_Thickness, 0);
	Vertices[37] = Vertices[13] + FVector(0, m_Thickness, 0);
	Vertices[38] = Vertices[35] - FVector(0, 0, LegsHeight);
	Vertices[39] = Vertices[36] - FVector(0, 0, LegsHeight);
	Vertices[40] = Vertices[37] - FVector(0, 0, LegsHeight);
	Vertices[41] = Vertices[13] - FVector(0, 0, LegsHeight);

	FVector mmp(-1, -1, 1); mmp.Normalize();
	FVector mpp(-1, 1, 1); mpp.Normalize();
	FVector ppp(1, 1, 1); ppp.Normalize();
	FVector pmp(1, -1, 1); pmp.Normalize();
	FVector mmz(-1, -1, 0); mmz.Normalize();
	FVector mpz(-1, 1, 0); mpz.Normalize();
	FVector ppz(1, 1, 0); ppz.Normalize();
	FVector pmz(1, -1, 0); pmz.Normalize();
	FVector mmm(-1, -1, -1); mmm.Normalize();
	FVector mpm(-1, 1, -1); mpm.Normalize();
	FVector ppm(1, 1, -1); ppm.Normalize();
	FVector pmm(1, -1, -1); pmm.Normalize();
	FVector mzz(-1, 0, 0); mzz.Normalize();
	FVector pzz(1, 0, 0); pzz.Normalize();

	const TArray<int32> Triangles({ 0,2,3,0,1,2 });

	const TArray<FVector>& Vertices0 = { Vertices[0], Vertices[1], Vertices[2], Vertices[3] };
	const TArray<FVector2D>& UV0 = { FVector2D(0, 0.5), FVector2D(0.1, 0.5), FVector2D(0.1, 0), FVector2D(0,0) };
	CreateMeshSection(0, Vertices0, Triangles, { mmp, mpp, ppp, pmp }, UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices1 = { Vertices[4], Vertices[5], Vertices[1], Vertices[0] };
	const TArray<FVector2D>& UV1 = { FVector2D(0.9, 1), FVector2D(1, 1), FVector2D(1, 0.5), FVector2D(0.9,0.5) };
	CreateMeshSection(1, Vertices1, Triangles, { mmz, mpp, mpp, mmp }, UV1, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices2 = { Vertices[5], Vertices[6], Vertices[2], Vertices[1] };
	const TArray<FVector2D>& UV2 = { FVector2D(0.6, 0.5), FVector2D(1, 0.5), FVector2D(1, 0), FVector2D(0.6,0) };
	CreateMeshSection(2, Vertices2, Triangles, { mpp, ppp, ppp, mpp }, UV2, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices3 = { Vertices[6], Vertices[7], Vertices[3], Vertices[2] };
	const TArray<FVector2D>& UV3 = { FVector2D(1, 1), FVector2D(0.9, 1), FVector2D(0.9,0.5), FVector2D(1, 0.5) };
	CreateMeshSection(3, Vertices3, Triangles, { ppp, pmz, pmp, ppp }, UV3, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices4 = { Vertices[7], Vertices[4], Vertices[0], Vertices[3] };
	const TArray<FVector2D>& UV4 = UV2;
	CreateMeshSection(4, Vertices4, Triangles, { pmz, mmz, mmp, pmp }, UV4, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices5 = { Vertices[5], Vertices[8], Vertices[9], Vertices[6] };
	const TArray<FVector2D>& UV5 = { FVector2D(0.1, 0.5), FVector2D(0.6, 0.5), FVector2D(0.6, 0), FVector2D(0.1,0) };
	CreateMeshSection(5, Vertices5, Triangles, { mpp, mpp, ppp, ppp }, UV5, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices6 = { Vertices[10], Vertices[11], Vertices[8], Vertices[4] };
	const TArray<FVector2D>& UV6 = { FVector2D(0, 0.6), FVector2D(0.6, 0.6), FVector2D(0.6, 0.5), FVector2D(0,0.5) };
	CreateMeshSection(6, Vertices6, Triangles, { mmz, mpz, mpp, mmz }, UV6, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices7 = { Vertices[11], Vertices[12], Vertices[9], Vertices[8] };
	const TArray<FVector2D>& UV7 = { FVector2D(0, 0), FVector2D(0, 0.5), FVector2D(0.1, 0.5), FVector2D(0.1,0) };
	CreateMeshSection(7, Vertices7, Triangles, { mpz, ppz, ppp, mpp }, UV7, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices8 = { Vertices[12], Vertices[13], Vertices[7], Vertices[9] };
	const TArray<FVector2D>& UV8 = { FVector2D(0.6, 0.6), FVector2D(0, 0.6), FVector2D(0,0.5), FVector2D(0.6, 0.5) };
	CreateMeshSection(8, Vertices8, Triangles, { ppz, pmz, pmz, ppp }, UV8, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices9 = { Vertices[13], Vertices[10], Vertices[4], Vertices[7] };
	const TArray<FVector2D>& UV9 = UV7;
	CreateMeshSection(9, Vertices9, Triangles, { pmz, mmz, mmz, pmz }, UV9, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices10 = { Vertices[13], Vertices[12], Vertices[11], Vertices[10] };
	const TArray<FVector2D>& UV10 = UV5;
	CreateMeshSection(10, Vertices10, Triangles, { pmz, ppz, mpz, mmz }, UV10, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices11 = { Vertices[17], Vertices[18], Vertices[14], Vertices[10] };
	const TArray<FVector2D>& UV11 = { FVector2D(0.8, 1), FVector2D(0.9, 1), FVector2D(0.9, 0.5), FVector2D(0.8, 0.5) };
	CreateMeshSection(11, Vertices11, Triangles, { mmz, mpz, mpm, mmz }, UV11, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices12 = { Vertices[18], Vertices[19], Vertices[15], Vertices[14] };
	const TArray<FVector2D>& UV12 = UV11;
	CreateMeshSection(12, Vertices12, Triangles, { mpz, ppz, ppm, pmz }, UV12, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices13 = { Vertices[19], Vertices[20], Vertices[16], Vertices[15] };
	const TArray<FVector2D>& UV13 = UV11;
	CreateMeshSection(13, Vertices13, Triangles, { ppz, pmz, pmm, ppm }, UV13, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices14 = { Vertices[20], Vertices[17], Vertices[10], Vertices[16] };
	const TArray<FVector2D>& UV14 = UV11;
	CreateMeshSection(14, Vertices14, Triangles, { pmz, mmz, mmz, pmm }, UV14, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices15 = { Vertices[24], Vertices[25], Vertices[11], Vertices[21] };
	const TArray<FVector2D>& UV15 = UV11;
	CreateMeshSection(15, Vertices15, Triangles, { mmz, mpz, mpz, mmm }, UV15, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices16 = { Vertices[25], Vertices[26], Vertices[22], Vertices[11] };
	const TArray<FVector2D>& UV16 = UV11;
	CreateMeshSection(16, Vertices16, Triangles, { mpz, ppz, ppm, pmm }, UV16, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices17 = { Vertices[26], Vertices[27], Vertices[23], Vertices[22] };
	const TArray<FVector2D>& UV17 = UV11;
	CreateMeshSection(17, Vertices17, Triangles, { ppz, pmz, pmm, ppm }, UV17, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices18 = { Vertices[27], Vertices[24], Vertices[21], Vertices[23] };
	const TArray<FVector2D>& UV18 = UV11;
	CreateMeshSection(18, Vertices18, Triangles, { pmz, mmz, mmm, pmm }, UV18, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices19 = { Vertices[31], Vertices[32], Vertices[29], Vertices[28] };
	const TArray<FVector2D>& UV19 = UV11;
	CreateMeshSection(19, Vertices19, Triangles, { mmz, mpz, mpm, mmm }, UV19, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices20 = { Vertices[32], Vertices[33], Vertices[12], Vertices[29] };
	const TArray<FVector2D>& UV20 = UV11;
	CreateMeshSection(20, Vertices20, Triangles, { mpz, ppz, ppz, mpm }, UV20, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices21 = { Vertices[33], Vertices[34], Vertices[30], Vertices[12] };
	const TArray<FVector2D>& UV21 = UV11;
	CreateMeshSection(21, Vertices21, Triangles, { ppz, pmz, pmm, ppz }, UV21, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices22 = { Vertices[34], Vertices[31], Vertices[28], Vertices[30] };
	const TArray<FVector2D>& UV22 = UV11;
	CreateMeshSection(22, Vertices22, Triangles, { pmz, mmz, mmm, pmm }, UV22, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices23 = { Vertices[38], Vertices[39], Vertices[36], Vertices[35] };
	const TArray<FVector2D>& UV23 = UV11;
	CreateMeshSection(23, Vertices23, Triangles, { mmz, mpz, mpm, mmm }, UV23, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices24 = { Vertices[39], Vertices[40], Vertices[37], Vertices[36] };
	const TArray<FVector2D>& UV24 = UV11;
	CreateMeshSection(24, Vertices24, Triangles, { mpz, ppz, ppm, mpm }, UV24, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices25 = { Vertices[40], Vertices[41], Vertices[13], Vertices[37] };
	const TArray<FVector2D>& UV25 = UV11;
	CreateMeshSection(25, Vertices25, Triangles, { ppz, pmz, pmz, ppm }, UV25, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	const TArray<FVector>& Vertices26 = { Vertices[41], Vertices[38], Vertices[35], Vertices[13] };
	const TArray<FVector2D>& UV26 = UV11;
	CreateMeshSection(26, Vertices26, Triangles, { pmz, mmz, mmm, pmz }, UV26, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	if (m_Material) {
		SetMaterial(0, m_Material);
		SetMaterial(1, m_Material);
		SetMaterial(2, m_Material);
		SetMaterial(3, m_Material);
		SetMaterial(4, m_Material);
		SetMaterial(5, m_Material);
		SetMaterial(6, m_Material);
		SetMaterial(7, m_Material);
		SetMaterial(8, m_Material);
		SetMaterial(9, m_Material);
		SetMaterial(10, m_Material);

		SetMaterial(11, m_Material);
		SetMaterial(12, m_Material);
		SetMaterial(13, m_Material);
		SetMaterial(14, m_Material);
		SetMaterial(15, m_Material);
		SetMaterial(16, m_Material);
		SetMaterial(17, m_Material);
		SetMaterial(18, m_Material);
		SetMaterial(19, m_Material);
		SetMaterial(20, m_Material);
		SetMaterial(21, m_Material);
		SetMaterial(22, m_Material);
		SetMaterial(23, m_Material);
		SetMaterial(24, m_Material);
		SetMaterial(25, m_Material);
		SetMaterial(26, m_Material);
	}
}
