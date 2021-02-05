// Fill out your copyright notice in the Description page of Project Settings.


#include "TableProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

int32 UTableProceduralMeshComponent::s_verticesCount = 36;

UTableProceduralMeshComponent::UTableProceduralMeshComponent() {
	m_Thickness = 10.f;
	m_origDX = m_origDY = 0.f;
}

void UTableProceduralMeshComponent::Generate(const TArray<FVector>& Vertices) {
	if (Vertices.Num() == 4) {
		m_Vertices.Reset();
		m_Vertices.AddUninitialized(UTableProceduralMeshComponent::s_verticesCount);
		UpdateVertices(Vertices);

		m_UV = { FVector2D(0,1), FVector2D(1,1), FVector2D(1,0), FVector2D(0,0) };

		m_origDX = Vertices[2].X - Vertices[0].X;
		m_origDY = Vertices[2].Y - Vertices[0].Y;

		if (m_Thickness > 0) {
			const float SideSize = m_Thickness / (m_Vertices[1].Y - m_Vertices[0].Y);
			m_Side0UV = { FVector2D(0, 1 + SideSize), FVector2D(1, 1 + SideSize), FVector2D(1,1), FVector2D(0,1) };
			m_Side1UV = { FVector2D(1 + SideSize,1), FVector2D(1 + SideSize, 0), FVector2D(1,0), FVector2D(1,1) };
			m_Side2UV = { FVector2D(1,-SideSize), FVector2D(0, -SideSize), FVector2D(0, 0), FVector2D(1,0) };
			m_Side3UV = { FVector2D(-SideSize,0), FVector2D(-SideSize, 1), FVector2D(0, 1), FVector2D(0,0) };
		}

		const TArray<FVector2D>& LegUV = { FVector2D(0,0), FVector2D(0.25,0), FVector2D(0.5,0), FVector2D(0.75,0), FVector2D(1,0), FVector2D(0,1), FVector2D(0.25,1), FVector2D(0.5,1), FVector2D(0.75,1), FVector2D(1,1) };

		FVector mmp(-1, -1, 1); mmp.Normalize();
		FVector mpp(-1, 1, 1); mpp.Normalize();
		FVector ppp(1, 1, 1); ppp.Normalize();
		FVector pmp(1, -1, 1); pmp.Normalize();
		FVector mmz(-1, -1, 0); mmz.Normalize();
		FVector mpz(-1, 1, 0); mpz.Normalize();
		FVector ppz(1, 1, 0); ppz.Normalize();
		FVector pmz(1, -1, 0); pmz.Normalize();

		const TArray<FVector>& LegNorm = { mmz, mpz, ppz, pmz, mmz, mmz, mpz, ppz, pmz, mmz };
		
		CreateMeshSection(0, { m_Vertices[0], m_Vertices[1], m_Vertices[2], m_Vertices[3] }, { 0,2,3, 0,1,2 }, TArray<FVector>({mmp, mpp, ppp, pmp}), m_UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(1, { m_Vertices[4], m_Vertices[5], m_Vertices[1], m_Vertices[0] }, { 0,2,3, 0,1,2 }, TArray<FVector>({mmz, mpz, mpp, mmp}), m_Side0UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(2, { m_Vertices[5], m_Vertices[6], m_Vertices[2], m_Vertices[1] }, { 0,2,3, 0,1,2 }, TArray<FVector>({mpz, ppz, ppp, mpp}), m_Side1UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(3, { m_Vertices[6], m_Vertices[7], m_Vertices[3], m_Vertices[2] }, { 0,2,3, 0,1,2 }, TArray<FVector>({ppz, pmz, pmp, ppp}), m_Side2UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(4, { m_Vertices[7], m_Vertices[4], m_Vertices[0], m_Vertices[3] }, { 0,2,3, 0,1,2 }, TArray<FVector>({pmz, mmz, mmp, pmp}), m_Side3UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(5, { m_Vertices[7], m_Vertices[6], m_Vertices[5], m_Vertices[4] }, { 0,2,3, 0,1,2 }, TArray<FVector>({pmz, ppz, mpz, mmz}), m_UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(6, { m_Vertices[4], m_Vertices[8], m_Vertices[9], m_Vertices[10],  m_Vertices[4], m_Vertices[11], m_Vertices[12], m_Vertices[13], m_Vertices[14], m_Vertices[11] }, { 5,1,0, 5,6,1, 6,2,1, 6,7,2, 7,3,2, 7,8,3, 8,4,3, 8,9,4 }, LegNorm, LegUV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(7, { m_Vertices[15], m_Vertices[5], m_Vertices[16], m_Vertices[17], m_Vertices[15], m_Vertices[18], m_Vertices[19], m_Vertices[20], m_Vertices[21], m_Vertices[18] }, { 5,1,0, 5,6,1, 6,2,1, 6,7,2, 7,3,2, 7,8,3, 8,4,3, 8,9,4 }, LegNorm, LegUV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(8, { m_Vertices[22], m_Vertices[23], m_Vertices[6], m_Vertices[24], m_Vertices[22], m_Vertices[25], m_Vertices[26], m_Vertices[27], m_Vertices[28], m_Vertices[25] }, { 5,1,0, 5,6,1, 6,2,1, 6,7,2, 7,3,2, 7,8,3, 8,4,3, 8,9,4 }, LegNorm, LegUV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		CreateMeshSection(9, { m_Vertices[29], m_Vertices[30], m_Vertices[31], m_Vertices[7], m_Vertices[29], m_Vertices[32], m_Vertices[33], m_Vertices[34], m_Vertices[35], m_Vertices[32] }, { 5,1,0, 5,6,1, 6,2,1, 6,7,2, 7,3,2, 7,8,3, 8,4,3, 8,9,4 }, LegNorm, LegUV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

		if (m_Material) {
			SetMaterial(0, m_Material);
			SetMaterial(1, m_Material);
			SetMaterial(2, m_Material);
			SetMaterial(3, m_Material);
			SetMaterial(4, m_Material);
			SetMaterial(5, m_Material);
		}

		if (m_LegsMaterial) {
			SetMaterial(6, m_LegsMaterial);
			SetMaterial(7, m_LegsMaterial);
			SetMaterial(8, m_LegsMaterial);
			SetMaterial(9, m_LegsMaterial);
		}
	}
}

void UTableProceduralMeshComponent::Update(const TArray<FVector>& Vertices) {
	if (m_Vertices.Num() == UTableProceduralMeshComponent::s_verticesCount) {
		bool newLowerLeft = Vertices[0] != m_Vertices[0];
		bool newLowerRight = Vertices[1] != m_Vertices[1];
		bool newUpperRight = Vertices[2] != m_Vertices[2];
		bool newUpperLeft = Vertices[3] != m_Vertices[3];

		const float D0X = (Vertices[0].X - m_Vertices[0].X) / m_origDX; // == D1X
		const float D0Y = (Vertices[0].Y - m_Vertices[0].Y) / m_origDY; // == D3Y
		const float D2X = (Vertices[2].X - m_Vertices[2].X) / m_origDX; // == D3X
		const float D2Y = (Vertices[2].Y - m_Vertices[2].Y) / m_origDY; // == D1Y

		UpdateVertices(Vertices);

		// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("%f, %f, %f, %f"), D0X, D0Y, D2X, D2Y));
		m_UV[0].X += D0Y;
		m_UV[0].Y -= D0X;
		m_UV[1].X += D2Y;
		m_UV[1].Y -= D0X;
		m_UV[2].X += D2Y;
		m_UV[2].Y -= D2X;
		m_UV[3].X += D0Y;
		m_UV[3].Y -= D2X;

		UpdateMeshSection(0, { m_Vertices[0], m_Vertices[1], m_Vertices[2], m_Vertices[3] }, TArray<FVector>(), m_UV, TArray<FColor>(), TArray<FProcMeshTangent>());
		if (newLowerLeft || newLowerRight) {
			m_Side0UV[0].X += D0Y;
			m_Side0UV[0].Y -= D0X;
			m_Side0UV[1].X += D2Y;
			m_Side0UV[1].Y -= D0X;
			m_Side0UV[2].X += D2Y;
			m_Side0UV[2].Y -= D0X;
			m_Side0UV[3].X += D0Y;
			m_Side0UV[3].Y -= D0X;
			UpdateMeshSection(1, { m_Vertices[4], m_Vertices[5], m_Vertices[1], m_Vertices[0] }, TArray<FVector>(), m_Side0UV, TArray<FColor>(), TArray<FProcMeshTangent>());
		}
		if (newLowerRight || newUpperRight) {
			m_Side1UV[0].X += D2Y;
			m_Side1UV[0].Y -= D0X;
			m_Side1UV[1].X += D2Y;
			m_Side1UV[1].Y -= D2X;
			m_Side1UV[2].X += D2Y;
			m_Side1UV[2].Y -= D2X;
			m_Side1UV[3].X += D2Y;
			m_Side1UV[3].Y -= D0X;
			UpdateMeshSection(2, { m_Vertices[5], m_Vertices[6], m_Vertices[2], m_Vertices[1] }, TArray<FVector>(), m_Side1UV, TArray<FColor>(), TArray<FProcMeshTangent>());
		}
		if (newUpperRight || newUpperLeft) {
			m_Side2UV[0].X += D2Y;
			m_Side2UV[0].Y -= D2X;
			m_Side2UV[1].X += D0Y;
			m_Side2UV[1].Y -= D2X;
			m_Side2UV[2].X += D0Y;
			m_Side2UV[2].Y -= D2X;
			m_Side2UV[3].X += D2Y;
			m_Side2UV[3].Y -= D2X;
			UpdateMeshSection(3, { m_Vertices[6], m_Vertices[7], m_Vertices[3], m_Vertices[2] }, TArray<FVector>(), m_Side2UV, TArray<FColor>(), TArray<FProcMeshTangent>());
		}
		if (newUpperLeft || newLowerLeft) {
			m_Side3UV[0].X += D0Y;
			m_Side3UV[0].Y -= D2X;
			m_Side3UV[1].X += D0Y;
			m_Side3UV[1].Y -= D0X;
			m_Side3UV[2].X += D0Y;
			m_Side3UV[2].Y -= D0X;
			m_Side3UV[3].X += D0Y;
			m_Side3UV[3].Y -= D2X;
			UpdateMeshSection(4, { m_Vertices[7], m_Vertices[4], m_Vertices[0], m_Vertices[3] }, TArray<FVector>(), m_Side3UV, TArray<FColor>(), TArray<FProcMeshTangent>());
		}
		UpdateMeshSection(5, { m_Vertices[7], m_Vertices[6], m_Vertices[5], m_Vertices[4] }, TArray<FVector>(), m_UV, TArray<FColor>(), TArray<FProcMeshTangent>());
		if (newLowerLeft) {
			UpdateMeshSection(6, { m_Vertices[4], m_Vertices[8], m_Vertices[9], m_Vertices[10],  m_Vertices[4], m_Vertices[11], m_Vertices[12], m_Vertices[13], m_Vertices[14], m_Vertices[11] }, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
		}
		if (newLowerRight) {
			UpdateMeshSection(7, { m_Vertices[15], m_Vertices[5], m_Vertices[16], m_Vertices[17], m_Vertices[15], m_Vertices[18], m_Vertices[19], m_Vertices[20], m_Vertices[21], m_Vertices[18] }, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
		}
		if (newUpperRight) {
			UpdateMeshSection(8, { m_Vertices[22], m_Vertices[23], m_Vertices[6], m_Vertices[24], m_Vertices[22], m_Vertices[25], m_Vertices[26], m_Vertices[27], m_Vertices[28], m_Vertices[25] }, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
		}
		if (newUpperLeft) {
			UpdateMeshSection(9, { m_Vertices[29], m_Vertices[30], m_Vertices[31], m_Vertices[7], m_Vertices[29], m_Vertices[32], m_Vertices[33], m_Vertices[34], m_Vertices[35], m_Vertices[32] }, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
		}
	}
}


void UTableProceduralMeshComponent::UpdateVertices(const TArray<FVector>& Vertices) {
	m_Vertices[0] = Vertices[0];
	m_Vertices[1] = Vertices[1];
	m_Vertices[2] = Vertices[2];
	m_Vertices[3] = Vertices[3];

	m_Vertices[4] = Vertices[0] - FVector(0, 0, m_Thickness);
	m_Vertices[5] = Vertices[1] - FVector(0, 0, m_Thickness);
	m_Vertices[6] = Vertices[2] - FVector(0, 0, m_Thickness);
	m_Vertices[7] = Vertices[3] - FVector(0, 0, m_Thickness);

	m_Vertices[8] = m_Vertices[4] + FVector(0, m_Thickness, 0);
	m_Vertices[9] = m_Vertices[4] + FVector(m_Thickness, m_Thickness, 0);
	m_Vertices[10] = m_Vertices[4] + FVector(m_Thickness, 0, 0);
	m_Vertices[11] = FVector(m_Vertices[4].X, m_Vertices[4].Y, 0);
	m_Vertices[12] = FVector(m_Vertices[8].X, m_Vertices[8].Y, 0);
	m_Vertices[13] = FVector(m_Vertices[9].X, m_Vertices[9].Y, 0);
	m_Vertices[14] = FVector(m_Vertices[10].X, m_Vertices[10].Y, 0);

	m_Vertices[15] = m_Vertices[5] + FVector(0, -m_Thickness, 0);
	m_Vertices[16] = m_Vertices[5] + FVector(m_Thickness, 0, 0);
	m_Vertices[17] = m_Vertices[5] + FVector(m_Thickness, -m_Thickness, 0);
	m_Vertices[18] = FVector(m_Vertices[15].X, m_Vertices[15].Y, 0);
	m_Vertices[19] = FVector(m_Vertices[5].X, m_Vertices[5].Y, 0);
	m_Vertices[20] = FVector(m_Vertices[16].X, m_Vertices[16].Y, 0);
	m_Vertices[21] = FVector(m_Vertices[17].X, m_Vertices[17].Y, 0);

	m_Vertices[22] = m_Vertices[6] + FVector(-m_Thickness, -m_Thickness, 0);
	m_Vertices[23] = m_Vertices[6] + FVector(-m_Thickness, 0, 0);
	m_Vertices[24] = m_Vertices[6] + FVector(0, -m_Thickness, 0);
	m_Vertices[25] = FVector(m_Vertices[22].X, m_Vertices[22].Y, 0);
	m_Vertices[26] = FVector(m_Vertices[23].X, m_Vertices[23].Y, 0);
	m_Vertices[27] = FVector(m_Vertices[6].X, m_Vertices[6].Y, 0);
	m_Vertices[28] = FVector(m_Vertices[24].X, m_Vertices[24].Y, 0);

	m_Vertices[29] = m_Vertices[7] + FVector(-m_Thickness, 0, 0);
	m_Vertices[30] = m_Vertices[7] + FVector(-m_Thickness, m_Thickness, 0);
	m_Vertices[31] = m_Vertices[7] + FVector(0, m_Thickness, 0);
	m_Vertices[32] = FVector(m_Vertices[29].X, m_Vertices[29].Y, 0);
	m_Vertices[33] = FVector(m_Vertices[30].X, m_Vertices[30].Y, 0);
	m_Vertices[34] = FVector(m_Vertices[31].X, m_Vertices[31].Y, 0);
	m_Vertices[35] = FVector(m_Vertices[7].X, m_Vertices[7].Y, 0);
}