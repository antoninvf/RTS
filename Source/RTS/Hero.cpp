// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"

#include "DrawDebugHelpers.h"
#include "NPC.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHero::AHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->GravityScale = 0;
	CameraComponent->SetRelativeLocation(FVector(0, 0, 500));
	SetActorRotation(FRotator(-30, 0, 0));

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(FInputModeGameAndUI());
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("Sideways", this, &AHero::MoveSideways);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AHero::DrawLine);
	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &AHero::MoveNPC);
}

void AHero::MoveForward(float Scale)
{
	AddMovementInput(GetActorForwardVector(), Scale);
}

void AHero::MoveSideways(float Scale)
{
	AddMovementInput(GetActorRightVector(), Scale);
}

void AHero::DrawLine()
{
	UE_LOG(LogTemp, Warning, TEXT("DrawLine called"));
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector Location, Direction;
	PlayerController->DeprojectMousePositionToWorld(Location, Direction);

	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Location + Direction * 5000;
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_LookAt);

	if (!HitResult.Location.IsZero() && HitResult.Actor.IsValid())
	{
		ANPC* Npc = Cast<ANPC>(HitResult.GetActor());
		if (Npc)
		{
			DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(10, 10, 10), FColor::Green, false, 2, 0, 1);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Selected an NPC")),
			                                 true);
			SelectedNPC = Npc;
		}
		else
		{
			DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(10, 10, 10), FColor::Red, false, 2, 0, 1);
		}
		// DrawDebugLine(GetWorld(), Location, HitResult.ImpactPoint, FColor::Red, false, 1, 0, 1);
	}
}

void AHero::MoveNPC()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveNPC called"));

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector Location, Direction;
	PlayerController->DeprojectMousePositionToWorld(Location, Direction);

	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Location + Direction * 5000;
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_LookAt);
	
	if (!HitResult.Location.IsZero() && HitResult.Actor.IsValid())
	{
		if (SelectedNPC)
		{
			ANPC* Npc = Cast<ANPC>(SelectedNPC);
			Npc->Moving(HitResult.ImpactPoint);
			DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(40, 40, 5), FColor::Green, false, 2, 0, 1);
			DrawDebugLine(GetWorld(), Npc->GetActorLocation(), HitResult.ImpactPoint, FColor::Blue, false, 1, 0, 1);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Going to destitation")),true);
		}
	}
}
