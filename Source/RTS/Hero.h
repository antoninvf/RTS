// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

#define ECC_LookAt ECC_GameTraceChannel1

UCLASS()
class RTS_API AHero : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	ANPC* SelectedNPC;

	// Sets default values for this character's properties
	AHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Scale);
	void MoveSideways(float Scale);
	
	void DrawLine();
	void MoveNPC();
};