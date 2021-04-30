#include "TAVisionComponent.h"


#include "DrawDebugHelpers.h"
#include "TAVisionTargetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Team18_AI/Team18_AIGameMode.h"

UTAVisionComponent::UTAVisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTAVisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<ATeam18_AIGameMode>(UGameplayStatics::GetGameMode(this));
}

void UTAVisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!VisionSettings)
		return;

	const FVector MyDirection = GetOwner()->GetActorForwardVector();
	const FVector MyOrigin = GetOwner()->GetActorLocation();

	// Debug Drawing
	if(bDebugDrawVision)
	{
		FVector const Right = MyDirection.RotateAngleAxis(VisionSettings->Angle, FVector::UpVector);
		FVector const Left = MyDirection.RotateAngleAxis(-VisionSettings->Angle, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), MyOrigin, MyOrigin + Right * VisionSettings->Distance, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), MyOrigin, MyOrigin + Left * VisionSettings->Distance, FLinearColor::Green);
	}

	// Checked Sensed Targets
	for(int i = SensedTargets.Num() - 1; i >= 0; i--)
	{
		UTAVisionTargetComponent* Target = SensedTargets[i];

		// Remove if nullptr or Being Destroyed
		if(!Target || (Target && Target->IsBeingDestroyed()))
		{
			SensedTargets.RemoveAt(i);
			continue;
		}

		// Can't See Target
		if(!IsPointVisible(Target->GetLocation(), MyOrigin, MyDirection, VisionSettings->Distance) || !CanSeeTarget(MyOrigin, Target->GetLocation(), Target))
		{
			FTAVisionResults Results;
			Results.SensedActor = Target->GetOwner();
			OnTargetLost.Broadcast(Results);
			SensedTargets.RemoveAt(i);
		}
	}

	// Check for New Targets
	TArray<UTAVisionTargetComponent*> ListOfTargets;
	GameMode->GetSensingTargets(ListOfTargets, GetOwner()->GetActorLocation(), VisionSettings->Distance);

	for(UTAVisionTargetComponent* Target : ListOfTargets)
	{
		if(!SensedTargets.Contains(Target) && IsPointVisible(Target->GetLocation(), MyOrigin, MyDirection, VisionSettings->Distance))
		{
			if(!CanSeeTarget(MyOrigin, Target->GetLocation(), Target))
				continue;
				
			SensedTargets.Add(Target);
			FTAVisionResults Results;
			Results.SensedActor = Target->GetOwner();
			OnTargetSensed.Broadcast(Results);
		}
	}
}

bool UTAVisionComponent::IsPointVisible(const FVector Point, const FVector Origin, const FVector Direction, const float Distance) const
{
	const float DistanceSq = FMath::Square(Distance);

	if(FVector::DistSquared(Origin, Point) > DistanceSq)
		return false;

	const FVector DirectionToTarget = (Point - Origin).GetSafeNormal();
	const float AsHalfRad = FMath::Cos(FMath::DegreesToRadians(VisionSettings->Angle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	return Dot > AsHalfRad;
}

bool UTAVisionComponent::CanSeeTarget(const FVector& Start, const FVector& End, UTAVisionTargetComponent* Target) const
{
	FHitResult Hit;

	TArray<AActor*> Ignores;
	Ignores.Add(GetOwner());
	Ignores.Add(Target->GetOwner());
	
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Vision_Trace")),
        true);
	TraceParams.AddIgnoredActors(Ignores);
	TraceParams.bReturnPhysicalMaterial = false;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	if(bDebugDrawVision)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 0, 0, 2);
		if(Hit.bBlockingHit)
		{
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5), FColor::Blue, false, 0, 0, 2);
		}
	}
	
	return Hit.bBlockingHit ? false : true;
}

