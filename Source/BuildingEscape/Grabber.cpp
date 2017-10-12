// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
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

	UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
		*PlayerViewpointLocation.ToString(), 
		*PlayerViewpointRotation.ToString()
	);

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

