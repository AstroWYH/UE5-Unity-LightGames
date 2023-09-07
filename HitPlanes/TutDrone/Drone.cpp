// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADrone::ADrone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	RootComponent = OutCollision;
	OutCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f)); // ���ô�С
	OutCollision->SetSimulatePhysics(true); // ����ģ������
	OutCollision->BodyInstance.bLockXRotation = true; // ˮƽ��б
	OutCollision->BodyInstance.bLockYRotation = true;
	//OutCollision->SetCollisionProfileName(TEXT("Pawn")); // ������ײΪPawn��

	// Mesh���ӵ�Actor 
	//RootComponent = Mesh;
	//Mesh->SetupAttachment(RootComponent);

	// �һ�
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OutCollision); // ���ûһ�������ײ

	// ����
	Paddle1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));

	// Paddle1->AttachToComponent(); // ������������ڹ��캯����ʹ��
	// �����󵽻һ���
	Paddle1->SetupAttachment(Mesh, TEXT("Paddle1"));
	Paddle2->SetupAttachment(Mesh, TEXT("Paddle2"));
	Paddle3->SetupAttachment(Mesh, TEXT("Paddle3"));
	Paddle4->SetupAttachment(Mesh, TEXT("Paddle4"));
	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);

	// ��ʼ�����ϵ�����
	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
	UpThruster->SetupAttachment(RootComponent); // ���ӵ��������
	//UpThruster->ThrustStrength = 1000.0f; // ����С
	UpThruster->ThrustStrength = 980.0f;
	UpThruster->SetAutoActivate(true); // ����������
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector())); // ����

	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
	ForwardThruster->SetupAttachment(RootComponent);
	ForwardThruster->ThrustStrength = 0.0f;
	ForwardThruster->SetAutoActivate(true);
	ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector())); // ����
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetInputAxisValue(TEXT("Lift")) == 0.0f) {
		UpThruster->ThrustStrength = 980.0f;
	}
	if (GetInputAxisValue(TEXT("Forward")) == 0.0f) {
		ForwardThruster->ThrustStrength = 0.0f;

		if (Mesh->GetRelativeRotation().Pitch != 0.0f){
			float currentPitch = Mesh->GetRelativeRotation().Pitch;
			Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime * 2.0, 0.0f, 0.0f));

			//if (FMath::Abs(Mesh->GetRelativeRotation().Pitch <= KINDA_SMALL_NUMBER) {
			if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER) {
				Mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			}
		}
	}

	RotatePaddle(DeltaTime);
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ADrone::doFire);
}

void ADrone::doFire()
{
	FTransform firePoint = Mesh->GetSocketTransform(TEXT("FiringSocket"));
	GetWorld()->SpawnActor<AMissle>(Bullet, firePoint);
}

void ADrone::Lift(float val)
{
	UpThruster->ThrustStrength += val * LiftAcc * GetWorld()->DeltaTimeSeconds;
	UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength, -LiftThrusMax, LiftThrusMax);
}

void ADrone::Forward(float val)
{
	ForwardThruster->ThrustStrength += val * ForwardAcc * GetWorld()->DeltaTimeSeconds;
	ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardThrusMax, ForwardThrusMax);

	float pitch = Mesh->GetRelativeRotation().Pitch;
	if (FMath::Abs(pitch) < 30.0f){
		Mesh->AddRelativeRotation(FRotator(-100.0f *  GetWorld()->DeltaTimeSeconds * val, 0.0f, 0.0f));
	}
	/*	�Ż���
		float valpitch = -val * pitch;
		if (valpitch < 30.0f)
		{
			Mesh->AddRelativeRotation(FRotator(-val * 100.0f * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f));
		}
	*/
}

void ADrone::Turn(float val)
{
	OutCollision->AddTorqueInDegrees(-this->GetActorUpVector() * val * TurnStrength);
}

void ADrone::RotatePaddle(float Delta)
{
	/*
		�����ǣ�Roll��Χ��X����ת
		�����ǣ�Pitch��Χ��Y����ת
		ƫ���ǣ�Yaw��Χ��Z����ת
	*/
	for (UStaticMeshComponent* paddle : Paddles) { // ����Yaw��ת Z
		paddle->AddRelativeRotation(FRotator(0.0f, Delta * PaddleRotateSpeed, 0.0f));
	}
}

