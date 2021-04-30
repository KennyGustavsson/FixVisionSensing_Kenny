// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "VisionSettings.generated.h"

UCLASS()
class TEAM18_AI_API UVisionSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Vision)
	float Angle = 60.0f;

	UPROPERTY(EditAnywhere, Category=Vision)
	float Distance = 2000.0f;
};
