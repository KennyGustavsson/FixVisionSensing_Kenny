#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Team18_AI/BlueprintFunctions/SoundEvent.h"
#include "Team18_AIGameMode.generated.h"

class UTAVisionTargetComponent;
class UTAListeneningComponent;

UCLASS(minimalapi)
class ATeam18_AIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATeam18_AIGameMode();
	virtual void Tick(float DeltaTime) override;
	
	void GetSensingTargets(TArray<UTAVisionTargetComponent*>& OutTargets, const FVector& Origin, float DistanceMin);
	TArray<UTAVisionTargetComponent*> VisionTargets;

	void GetHeardNoise(TArray<FSoundEventData*>& OutSoundEvents, const FVector& Origin, float DistanceMin);
	void AddNoise(FSoundEventData SoundData);
	TArray<FSoundEventData> ActiveNoise;

	FVector GetClosestCoverPoint(FVector Location);
	void AddCoverPoint(FVector Location);
	TArray<FVector> CoverPoints;
};
