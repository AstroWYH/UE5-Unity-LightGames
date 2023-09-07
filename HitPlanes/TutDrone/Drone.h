// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Missle.h"
#include "Drone.generated.h"

UCLASS()
class TUTDRONE_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

	// 碰撞
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* OutCollision;

	// 灰机
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	// 旋桨
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle1;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle2;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle3;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle4;

	// 飞行的推力
	UPROPERTY(VisibleAnywhere)
		class UPhysicsThrusterComponent* UpThruster; // 向上
	UPROPERTY(VisibleAnywhere)
		class UPhysicsThrusterComponent* ForwardThruster; // 向前

	// 上下加速的数值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float LiftAcc = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float LiftThrusMax = 2000.0f;
	
	// 前后加速的数值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float ForwardAcc = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float ForwardThrusMax = 2000.0f;

	// 转动角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float TurnStrength = 500000.0f;

	// 转动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float PaddleRotateSpeed = 500.0f;

	// 子弹
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMissle> Bullet;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void doFire();

private:
	UFUNCTION()
		void Lift(float val);
	UFUNCTION()
		void Forward(float val);
	UFUNCTION()
		void Turn(float val);

	TArray<UStaticMeshComponent*> Paddles;
	void RotatePaddle(float Delta);
};
