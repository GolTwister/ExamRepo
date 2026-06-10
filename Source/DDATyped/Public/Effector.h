#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Effector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApplyModifier, float, ModifierValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnApplyModifierInterp, float, ModifierValue, float, Duration);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DDATYPED_API UEffector : public UActorComponent
{
	GENERATED_BODY()

public:
    UEffector();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effector")
    FString EffectorName;

    UFUNCTION(BlueprintCallable, Category = "Effector")
    void ApplyModifier(float Value);

    //Applying Modifier by Effector
    UPROPERTY(BlueprintAssignable, Category = "Effector")
    FOnApplyModifier OnApplyModifier;

    UFUNCTION(BlueprintCallable, Category = "Effector")
    void ApplyModifierInterp(float TargetValue, float Duration);

    //Applying Modifier by Effector with interpolation
    UPROPERTY(BlueprintAssignable, Category = "Effector")
    FOnApplyModifierInterp OnApplyModifierInterp;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Effector")
    void ResetToDefault();
    virtual void ResetToDefault_Implementation();

    UFUNCTION(BlueprintPure, Category = "Effector")
    float GetCurrentValue() const { return CurrentValue; }

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effector")
    float CurrentValue = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effector")
    float DefaultValue = 0.0f;

    float TargetValueInterp = 0.0f;
    float InterpDuration = 0.0f;
    float InterpTime = 0.0f;
    FTimerHandle InterpTimerHandle;
};
