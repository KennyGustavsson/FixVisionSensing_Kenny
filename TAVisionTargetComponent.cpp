#include "TAVisionTargetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Team18_AI/Team18_AIGameMode.h"

void UTAVisionTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	auto GameMode = Cast<ATeam18_AIGameMode>(UGameplayStatics::GetGameMode(this));
	if(GameMode)
		GameMode->VisionTargets.Add(this);
}

void UTAVisionTargetComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	auto GameMode = Cast<ATeam18_AIGameMode>(UGameplayStatics::GetGameMode(this));
	if(GameMode)
		GameMode->VisionTargets.Remove(this);
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

FVector UTAVisionTargetComponent::GetLocation() const
{
	return GetOwner()->GetActorLocation();
}

