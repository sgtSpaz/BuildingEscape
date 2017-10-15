// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT
#define PTR

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("GrabPressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FString OwnerName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	///look for attached Phys handle
	PTR PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
	}
	else
	{
	UE_LOG(LogTemp, Error, TEXT("No phys handle on %s"), *OwnerName);
	};

	///look for attached Phys handle
	PTR InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent found on %s"), *OwnerName);
		InputComponent->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{		
		UE_LOG(LogTemp, Error, TEXT("No input component on %s"), *OwnerName);
	};
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get player viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation
	);
/*
	UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
		*PlayerViewpointLocation.ToString(), 
		*PlayerViewpointRotation.ToString()
	);
*/
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	///setup query params
	FCollisionQueryParams QueryTraceParams (FName(TEXT("")), false, GetOwner());
	
	///draw a red trace in the world to visualize
	//DrawDebugLine(GetWorld(), PlayerViewpointLocation, LineTraceEnd, FColor(255, 0, 0), false, -1, 0, 12.f);
	

	///line-trace (raycast) to a reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams (ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams (QueryTraceParams)
		);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s"),	*(ActorHit->GetName())	);
	}
	
	///see what we hit
}

