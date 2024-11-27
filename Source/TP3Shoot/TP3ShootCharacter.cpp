// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP3ShootCharacter.h"

#include "AIControllerBase.h"
#include "Dead.h"
#include "Health.h"
#include "ProjectileBeam.h"
#include "Team.h"
#include "Zombie.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/STextScroller.h"


//////////////////////////////////////////////////////////////////////////
// ATP3ShootCharacter

ATP3ShootCharacter::ATP3ShootCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create SK_Gun
	SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	SK_Gun->SetupAttachment(GetMesh());
	// Set parent socket
	SK_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));

	//Create health component
	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));
	Dead = CreateDefaultSubobject<UDead>(TEXT("Dead"));
	Team = CreateDefaultSubobject<UTeam>(TEXT("Team"));

	// Create stimulus component
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	StimuliSource->RegisterWithPerceptionSystem();
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP3ShootCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATP3ShootCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATP3ShootCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATP3ShootCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATP3ShootCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATP3ShootCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATP3ShootCharacter::TouchStopped);

	// Aiming 
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &ATP3ShootCharacter::Aim);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &ATP3ShootCharacter::StopAiming);

	// Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATP3ShootCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATP3ShootCharacter::StopFiring);

	// Boost Speed
	PlayerInputComponent->BindAction("BoostSpeed", IE_Pressed, this, &ATP3ShootCharacter::BoostSpeed);
	PlayerInputComponent->BindAction("BoostSpeed", IE_Released, this, &ATP3ShootCharacter::RemoveSpeedBoost);
}


void ATP3ShootCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATP3ShootCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ATP3ShootCharacter::Aim()
{
	IsAiming = true;
}

void ATP3ShootCharacter::StopAiming()
{
	IsAiming = false;
}

void ATP3ShootCharacter::Fire()
{
	FVector Start, LineTraceEnd;

	if (!SK_Gun || !FollowCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SK_Gun or FollowCamera component in Fire()"));
		return;
	}

	//No shoot if not visible
	if (IsHidden()) return;

	Start = SK_Gun->GetSocketLocation("MuzzleFlash");

	LineTraceEnd = GetCameraRaycastHitLocation();
	if (LineTraceEnd == FVector::ZeroVector) LineTraceEnd = Start + FollowCamera->GetForwardVector() * 10000;

	// Raycast (line trace)
	FHitResult HitResult; // Structure to hold hit result information
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore self

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, LineTraceEnd, ECC_Visibility, CollisionParams))
	{
		AActor* HitActor = HitResult.GetActor();
		FVector ImpactPoint = HitResult.ImpactPoint;
		
		// If the trace hit something, log the hit information
		UE_LOG(LogType, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
		//UE_LOG(LogType, Warning, TEXT("Hit Location: %s"), *ImpactPoint.ToString());

		CheckIfZombie(HitActor);
		//CheckIfFiringApplyForce(HitActor, Start, ImpactPoint);
		//CheckIfCharacter(HitActor);

		// Optionally, you can trigger some effects, like spawning particles at the hit location
		FireParticle(SK_Gun->GetSocketLocation("MuzzleFlash"), ImpactPoint); // Or spawn effects based on the hit
	}
	else
	{
		// If nothing was hit, you can fire your particle effect at the end of the line trace
		FireParticle(SK_Gun->GetSocketLocation("MuzzleFlash"), LineTraceEnd);
	}
	
	//DrawDebugLine(GetWorld(), SK_Gun->GetSocketLocation("MuzzleFlash"), LineTraceEnd, FColor::Red, false, 0.2f, 0, 0.7f);
	
}

void ATP3ShootCharacter::FireStraight()
{
	FVector Start, LineTraceEnd;

	if (!SK_Gun)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SK_Gun component in FireStraight()"));
		return;
	}

	// No shoot if not visible
	if (IsHidden()) return;

	// Get the muzzle location
	Start = SK_Gun->GetSocketLocation("MuzzleFlash");

	// Get the forward vector of the actor that owns the SK_Gun component (this is typically the character)
	FVector ForwardVector = GetActorForwardVector();  // Get the forward vector of the actor
	LineTraceEnd = Start + ForwardVector * 10000;  // Adjust the range as needed

	// Raycast (line trace)
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore self

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, LineTraceEnd, ECC_Visibility, CollisionParams))
	{
		AActor* HitActor = HitResult.GetActor();
		FVector ImpactPoint = HitResult.ImpactPoint;

		// Check if the hit actor is valid and apply force or other logic
		//CheckIfZombie(HitActor);
		CheckIfFiringApplyForce(HitActor, Start, ImpactPoint);
		CheckIfCharacter(HitActor);

		// Optionally, trigger effects at the hit location
		FireParticle(SK_Gun->GetSocketLocation("MuzzleFlash"), ImpactPoint);
	}
	else
	{
		// If nothing was hit, fire the particle effect at the end of the trace
		FireParticle(SK_Gun->GetSocketLocation("MuzzleFlash"), LineTraceEnd);
	}

	// Optionally, you can draw a debug line to visualize the shot
	//DrawDebugLine(GetWorld(), Start, LineTraceEnd, FColor::Red, false, 0.2f, 0, 0.7f);
}



FVector ATP3ShootCharacter::GetCameraRaycastHitLocation()
{
	if (!FollowCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing FollowCamera component in GetCameraRaycastHitLocation()"));
		return FVector::ZeroVector;
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector LineTraceEnd = Start + (ForwardVector * 10000);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore self

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, LineTraceEnd, ECC_Visibility, CollisionParams))
	{
		return HitResult.ImpactPoint; // Return the location of the hit
	}

	// If no hit, return zero vector
	return FVector::ZeroVector;
}


void ATP3ShootCharacter::BeginPlay()
{
	Super::BeginPlay();

	BeamPool = Cast<AProjectilePool>(UGameplayStatics::GetActorOfClass(GetWorld(), AProjectilePool::StaticClass()));
}

void ATP3ShootCharacter::CheckIfFiringApplyForce(const AActor* HitActor, const FVector FiringLocation, const FVector HitLocation)
{
	if (!HitActor->IsA<APawn>())
	{
		UPrimitiveComponent* PhysicsComponent = HitActor->GetComponentByClass<UPrimitiveComponent>();
		if (PhysicsComponent && PhysicsComponent->IsSimulatingPhysics())
		{
			FVector ForceDirection = (HitActor->GetActorLocation() - FiringLocation).GetSafeNormal(); 
			FVector ForceMagnitude = ForceDirection * FiringForce; 
			
			//UE_LOG(LogType, Warning, TEXT("Apply force"));
			PhysicsComponent->AddImpulseAtLocation(ForceMagnitude, HitLocation);
				
		}
	}
}

void ATP3ShootCharacter::CheckIfCharacter(const AActor* HitActor)
{
	const ATP3ShootCharacter* character = Cast<ATP3ShootCharacter>(HitActor);

	if (character && !Team->IsSameTeam(character->Team->teamId))
	{
		character->NotifyHitByRaycast(FiringDamage);

		// Get the AI Controller and cast it to AAIControllerBase
		if (AAIControllerBase* AIController = Cast<AAIControllerBase>(character->GetController()))
		{
			if (AIController->BlackboardComponent)
			{
				UObject* target = AIController->BlackboardComponent->GetValueAsObject("TargetActor");

				if(target == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("No target"));
					
					AIController->BlackboardComponent->SetValueAsObject("TargetActor", this);
				}
			}
		}
	}
}

void ATP3ShootCharacter::CheckIfZombie(const AActor* HitActor)
{
	if (const AZombie* Zombie = Cast<AZombie>(HitActor))
	{
		Zombie->NotifyHitByRaycast(FiringDamage);
	}
}

void ATP3ShootCharacter::NotifyHitByRaycast(float firingDamage) const
{
	Health->AddHealth(-firingDamage);
}


void ATP3ShootCharacter::StartFiring()
{
	Fire();
	GetWorldTimerManager().SetTimer(FireTimer,this,&ATP3ShootCharacter::Fire, FiringRate,true);
}

void ATP3ShootCharacter::StopFiring()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}


void ATP3ShootCharacter::BoostSpeed()
{
	// Set Max walking speed to 800
	GetCharacterMovement()->MaxWalkSpeed = 800.f;

	GetWorld()->GetTimerManager().SetTimer(BoostSpeedTimer, [&]()
		{
			// Set Max walking speed to 500
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
			
			// Clear existing timer boost speed
			GetWorldTimerManager().ClearTimer(BoostSpeedTimer);

		}, 4, false);
}

void ATP3ShootCharacter::RemoveSpeedBoost()
{
	// Set Max walking speed to 500
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}


void ATP3ShootCharacter::FireParticle(FVector Start, FVector Impact)
{
	if (!ParticleStart || !ParticleImpact || !BeamPool)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ParticleStart, ParticleImpact, or BeamPool in FireParticle()"));
		return;
	}

	FTransform ParticleT;

	ParticleT.SetLocation(Start);

	ParticleT.SetScale3D(FVector(0.01, 0.01, 0.01));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleStart, ParticleT, true);

	// Spawn particle at impact point
	ParticleT.SetLocation(Impact);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleImpact, ParticleT, true);

	AActor* actor =  BeamPool->GetThrowable();

	if (AProjectileBeam* projectileBeam = Cast<AProjectileBeam>(actor))
	{
		projectileBeam->SetActorLocation(Start);
		const float Distance = FVector::Dist(Start, Impact);
		const FRotator Rotator = (Impact - Start).Rotation();
		projectileBeam->SetActorRotation(Rotator);
		projectileBeam->Activates(Distance, Team->GetTeamId());
	}
}

void ATP3ShootCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATP3ShootCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATP3ShootCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATP3ShootCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
