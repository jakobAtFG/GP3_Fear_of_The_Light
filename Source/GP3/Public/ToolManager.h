// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolManager.generated.h"

struct FInputActionValue;
class UBatteryComponent;

UENUM(BlueprintType)
enum class ETool : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	RADAR UMETA(DisplayName = "RADAR"),
	HEAT_VISION UMETA(DisplayName = "HEAT_VISION"),
	EMP UMETA(DisplayName = "EMP"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchToRadar);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchToEmp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchToHeatVision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnToolToggled, bool, ToolToggleOn, ETool, CurrentTool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToolSwitched, ETool, CurrentTool);

class AGP3Character;
class UGP3SaveGame;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_API UToolManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "ToolManager")
	TArray<ETool> AvailableTools;
	

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* ItemScrollerUp;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* ItemScrollerDown;

public:
	// Sets default values for this component's properties
	UToolManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ToolManager")
	ETool CurrentTool = ETool::NONE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGP3Character* OwnerCharacter = nullptr;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ToolManager")
	void AddTool(ETool Tool);

	UFUNCTION(BlueprintCallable, Category = "ToolManager")
	void EquipTool();

	UFUNCTION(BlueprintCallable)
	void SwitchTool(const FInputActionValue& Value);

	UFUNCTION()
	ETool GetCurrentTool() const { return CurrentTool; }

	UPROPERTY(BlueprintAssignable)
	FSwitchToRadar OnSwitchedToRadar;

	UPROPERTY(BlueprintAssignable)
	FSwitchToEmp OnSwitchedToEmp;

	UPROPERTY(BlueprintAssignable)
	FSwitchToHeatVision OnSwitchedToHeatVision;

	UPROPERTY(BlueprintAssignable)
	FOnToolToggled OnToolToggled;

	UPROPERTY(BlueprintAssignable)
	FOnToolSwitched OnToolSwitched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ToolToggleOn = false;

	UFUNCTION(BlueprintCallable)
	void SwitchToolRadar(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SwitchToolHeatVision(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SwitchToolEmp(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	bool bIsHoldingTool();

	UFUNCTION(BlueprintCallable)
	void UnequipTool();

private:
	UPROPERTY(EditDefaultsOnly)
	int Index;

	UPROPERTY(VisibleAnywhere)
	UBatteryComponent* BatteryComponent = nullptr;

	UFUNCTION()
	void HandleSaveRequested(UGP3SaveGame* SaveGame);

	UFUNCTION()
	void HandleSaveDataLoaded(UGP3SaveGame* SaveGame);
	
};
