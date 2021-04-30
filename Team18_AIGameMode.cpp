#include "Team18_AIGameMode.h"

#include "DrawDebugHelpers.h"
#include "Team18_AIHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "AI/Sensing/TAVisionTargetComponent.h"

ATeam18_AIGameMode::ATeam18_AIGameMode() : Super()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Game/BP/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = ATeam18_AIHUD::StaticClass();
}

void ATeam18_AIGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for(int i = ActiveNoise.Num() - 1; i >= 0; i--)
	{
		if(ActiveNoise[i].LifeTime <= 0)
		{
			ActiveNoise.RemoveAt(i);
			continue;
		}

		if(ActiveNoise[i].bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), ActiveNoise[i].Location, ActiveNoise[i].Radius, 12, FColor::Blue, false, 0, 3);
		}

		ActiveNoise[i].LifeTime -= DeltaTime;
		ActiveNoise[i].Radius += ActiveNoise[i].Growth * DeltaTime;
	}
}

void ATeam18_AIGameMode::GetSensingTargets(TArray<UTAVisionTargetComponent*>& OutTargets, const FVector& Origin, float DistanceMin)
{
	const float DistanceMinSq = FMath::Square(DistanceMin);
	for (UTAVisionTargetComponent* Target : VisionTargets)
	{
		if(!Target)
			continue;

		float DistanceSq = FVector::DistSquared(Target->GetOwner()->GetActorLocation(), Origin);
		if(DistanceSq < DistanceMinSq)
		{
			OutTargets.Add(Target);
		}
	}
}

void ATeam18_AIGameMode::GetHeardNoise(TArray<FSoundEventData*>& OutSoundEvents, const FVector& Origin, float DistanceMin)
{
	const float DistanceMinSq = FMath::Square(DistanceMin);

	for(int i = 0; i < ActiveNoise.Num(); i++)
	{
		float DistanceSq = FVector::DistSquared(ActiveNoise[i].Location, Origin);
		if(DistanceSq < DistanceMinSq)
		{
			OutSoundEvents.Add(&ActiveNoise[i]);
		}
	}
}

void ATeam18_AIGameMode::AddNoise(FSoundEventData SoundData)
{	
	ActiveNoise.Add(SoundData);
}

FVector ATeam18_AIGameMode::GetClosestCoverPoint(FVector Location)
{
	if(CoverPoints.Num() == 0)
		return FVector::ZeroVector;
	
	int BestIndex = 0;

	for (int i = 1; i < CoverPoints.Num(); i++)
	{
		float const DistanceSq1 = FVector::DistSquared(CoverPoints[BestIndex], Location);
		float const DistanceSq2 = FVector::DistSquared(CoverPoints[i], Location);

		if(DistanceSq1 > DistanceSq2)
		{
			BestIndex = i;
		}
	}

	return CoverPoints[BestIndex];
}

void ATeam18_AIGameMode::AddCoverPoint(FVector Location)
{
	CoverPoints.Add(Location);
}
