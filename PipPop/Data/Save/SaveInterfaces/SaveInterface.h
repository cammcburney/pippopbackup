// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/AppearanceStructs.h"
#include "SaveInterface.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()
	
	FName GetPlayerName() const {return PlayerName;}

	TMap<EAppearance, int32> GetCharacterMeshes() const {return CharacterMeshes;}

	TMap<EAppearance, int32> GetCharacterMaterials() const {return CharacterMaterials;}
	
	int32 GetMeshIndex(const EAppearance AppearanceEnum) const {return CharacterMeshes.FindRef(AppearanceEnum);}

	int32 GetMaterialIndex(const EAppearance AppearanceEnum) const {return CharacterMaterials.FindRef(AppearanceEnum);}
	
	void SetPlayerName(const FName& NewPlayerName) {PlayerName = NewPlayerName;}

	void SetAppearanceMesh(const EAppearance Member, const int32 MemberIndex) {CharacterMeshes.Add(Member, MemberIndex);}

	void SetAppearanceMaterial(const EAppearance Member, const int32 MemberIndex) {CharacterMaterials.Add(Member, MemberIndex);}

	FPlayerSaveData()
	{
		for (EAppearance Appearance : TEnumRange<EAppearance>())
		{
			CharacterMeshes.Add(Appearance, 0);
			CharacterMaterials.Add(Appearance, 0);
		}
	}
	
private:

	UPROPERTY()
	FName PlayerName = FName(NAME_None);

	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	TMap<EAppearance, int32> CharacterMeshes;
	
	UPROPERTY()
	TMap<EAppearance, int32> CharacterMaterials;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIPPOP_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
