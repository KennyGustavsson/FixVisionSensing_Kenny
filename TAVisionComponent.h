#pragma once

#include "CoreMinimal.h"

#include "VisionSettings.h"
#include "Components/ActorComponent.h"
#include "TAVisionComponent.generated.h"

class UTAVisionTargetComponent;
class ATeam18_AIGameMode;

USTRUCT(BlueprintType)
struct FTAVisionResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AActor* SensedActor = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTAVisionDelegate, const FTAVisionResults&, Results);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class TEAM18_AI_API UTAVisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTAVisionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool IsPointVisible(const FVector Point, const FVector Origin, const FVector Direction, const float Distance) const;
	bool CanSeeTarget(const FVector& Start, const FVector& End, UTAVisionTargetComponent* Target) const;

	UPROPERTY(EditAnywhere, Category=Vision)
	UVisionSettings* VisionSettings = nullptr;
	
	UPROPERTY(EditAnywhere, Category=Debug)
	bool bDebugDrawVision = false;

	UPROPERTY(Transient)
	TArray<UTAVisionTargetComponent*> SensedTargets;

	UPROPERTY(BlueprintAssignable)
	FTAVisionDelegate OnTargetSensed;

	UPROPERTY(BlueprintAssignable)
	FTAVisionDelegate OnTargetLost;
	
private:
	ATeam18_AIGameMode* GameMode;
};
