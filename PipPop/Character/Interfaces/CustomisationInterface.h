// Copyright Hawthorn House Games LTD

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CustomisationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCustomisationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIPPOP_API ICustomisationInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(UPrimitiveComponent* InteractedComponent);
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) {}

};
