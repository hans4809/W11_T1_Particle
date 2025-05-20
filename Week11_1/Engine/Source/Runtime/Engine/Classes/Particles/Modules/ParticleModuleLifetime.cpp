#include "ParticleModuleLifetime.h"
#include "Runtime/Engine/Particles/ParticleEmitterInstances.h"
#include <Particles/ParticleMacros.h>
#include "Core/Math/RandomStream.h"
void UParticleModuleLifeTime::InitializeDefaults()
{
    // FIXME : 테스트용 uniform 타입 생성 기본값으로 변경.
    UDistributionFloatUniform* DefaultDistribution = FObjectFactory::ConstructObject<UDistributionFloatUniform>(nullptr);
    DefaultDistribution->MinValue = 0.0f;
    DefaultDistribution->MaxValue = 1.0f;
    Lifetime.Distribution = DefaultDistribution;
}

void UParticleModuleLifeTime::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, float Interp)
{
    const int32 ActiveCount = Owner->ActiveParticles;
    const int32 ParticleStride = Owner->ParticleStride;
    uint8* ParticleData = Owner->ParticleData;

    FRandomStream Rand;
    Rand.Initialize(12345);

    for (int32 i = 0; i < ActiveCount; ++i)
    {
        uint8* Address = ParticleData + i * ParticleStride;
        DECLARE_PARTICLE_PTR(Particle, Address);
        Particle->Lifetime = UParticleModuleLifeTime::Lifetime.GetValue(&Rand);
        UE_LOG(LogLevel::Display, "Lifetime Value : %f", Particle->Lifetime);
    }

}

EModuleType UParticleModuleLifeTime::GetType() const
{
    return EModuleType::Spawn;

}


