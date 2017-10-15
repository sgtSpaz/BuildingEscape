// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"


#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT
#define PTR

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	OwnerName = GetOwner()->GetName();
	FindPhysHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysHandleComponent()
{
	PTR PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No phys handle on %s"), *OwnerName);
	};
}

void UGrabber::SetupInputComponent()
{
	PTR InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
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
	


	//if phys handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	//move the object we're holding
	
	
	///see what we hit
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("GrabPressed"));
	//TODO line trace and see if we reach any actors with phys body collision channel set
	auto HitResult = GetFirstPhysBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	//TODO if we hit something, then attach phys handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), FRotator(0));
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
	//TODO release phys handle
	auto HitResult = GetFirstPhysBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

const FHitResult UGrabber::GetFirstPhysBodyInReach()
{
	///draw a red trace in the world to visualize
	//DrawDebugLine(GetWorld(), GetReachLineStart(), GetReachLineEnd(), FColor(255, 0, 0), false, -1, 0, 12.f);

	///line-trace (raycast) to a reach distance
	FHitResult HitResult;
	FCollisionQueryParams QueryTraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(QueryTraceParams)
	);

	return HitResult;
}

///used for player center and player reach end
FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(	OUT PlayerViewpointLocation,OUT PlayerViewpointRotation);

	return PlayerViewpointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(	OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}
