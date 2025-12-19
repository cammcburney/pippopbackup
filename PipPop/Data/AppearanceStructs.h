#pragma once

#include "CoreMinimal.h"
#include "AppearanceStructs.generated.h"

USTRUCT(BlueprintType)
struct PIPPOP_API FAppearanceInfo: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SectionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<USkeletalMesh>> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UMaterialInterface>> Material;
};