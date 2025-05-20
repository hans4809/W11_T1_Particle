#include "ParticleModuleColor.h"
#include <Particles/ParticleMacros.h>
#include "Runtime/Engine/Particles/ParticleEmitterInstances.h"

void UParticleModuleColor::InitializeDefaults()
{
    // FIXME : color alpha 구현
    /*UDistributionFloatUniform* DefaultDistributionFloat = FObjectFactory::ConstructObject<UDistributionFloatUniform>(nullptr);
    DefaultDistributionFloat->MinValue = 0.0f;
    DefaultDistributionFloat->MaxValue = 1.0f;
    StartAlpha.Distribution = DefaultDistributionFloat;
    for (int i = 0; i < 10; i++)
    {
        float t = StartAlpha.GetValue();
        t = t + 1;
    }*/

    // 초기값
    //StartColor.StartColor = FVector(1.0f, 1.0f, 1.0f);

    UDistributionVectorUniform* DefaultDistributionVector = FObjectFactory::ConstructObject<UDistributionVectorUniform>(nullptr);
    DefaultDistributionVector->MinValue = FVector::ZeroVector;
    DefaultDistributionVector->MaxValue = FVector::OneVector;
    StartColor.Distribution = DefaultDistributionVector;
}

void UParticleModuleColor::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, float Interp)
{
    SPAWN_INIT
    
    for(int32 i = 0; i < ActiveCount; ++i) {
        uint8* Address = ParticleData + i * ParticleStride;
        DECLARE_PARTICLE_PTR(Particle, Address);

        FVector ColorVec = StartColor.GetValue();
        Particle->Color = FColor(
            FMath::Clamp(int32(ColorVec.X * 255), 0, 255),
            FMath::Clamp(int32(ColorVec.Y * 255), 0, 255),
            FMath::Clamp(int32(ColorVec.Z * 255), 0, 255),
            255 // alpha
        );
    }
}

void UParticleModuleColor::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{

}

EModuleType UParticleModuleColor::GetType() const
{
    return EModuleType();
}
