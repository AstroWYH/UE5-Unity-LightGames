// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetGenerateOverlapEvents(true);
	this->OnDestroyed.AddDynamic(this, &AEnemy::DestroyHandler);
}

void AEnemy::DestroyHandler(AActor* DestroyedActor)
{
	// ��ըλ��
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, this->GetActorTransform());
	// ��ը��Ч
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, this->GetActorLocation(), 2.0f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->AddActorWorldOffset(FVector(0.0f, 0.0f, 1.0f) * upSpeed * DeltaTime);
}

