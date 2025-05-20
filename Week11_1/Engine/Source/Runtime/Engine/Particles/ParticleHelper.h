#pragma once
#include "Math/Vector.h"
#include "Math/Color.h"
#include "Math/Vector4.h"

class UMaterial;
struct FParticleRequiredModule;
struct FTexture;
/**
 * Per-particle data sent to the GPU.
 */
struct FParticleSpriteVertex
{
    /** The position of the particle. */
    FVector Position;
    /** The relative time of the particle. */
    float RelativeTime;
    /** The previous position of the particle. */
    FVector	OldPosition;
    /** Value that remains constant over the lifetime of a particle. */
    float ParticleId;
    /** The size of the particle. */
    FVector2D Size;
    /** The rotation of the particle. */
    float Rotation;
    /** The sub-image index for the particle. */
    float SubImageIndex;
    /** The color of the particle. */
    FLinearColor Color;
};

// Per-particle data sent to the GPU.
struct FMeshParticleInstanceVertex
{
    /** The color of the particle. */
    FLinearColor Color;

    /** The instance to world transform of the particle. Translation vector is packed into W components. */
    FVector4 Transform[3];

    /** The velocity of the particle, XYZ: direction, W: speed. */
    FVector4 Velocity;

    /** The sub-image texture offsets for the particle. */
    int16 SubUVParams[4];

    /** The sub-image lerp value for the particle. */
    float SubUVLerp;

    /** The relative time of the particle. */
    float RelativeTime;
};



enum EDynamicEmitterType
{
    DET_Unknown = 0,
    DET_Sprite,
    DET_Mesh,
    DET_Beam2,
    DET_Ribbon,
    DET_AnimTrail,
    DET_Custom
};



/*-----------------------------------------------------------------------------
    FBaseParticle
-----------------------------------------------------------------------------*/
struct FBaseParticle
{
    FVector    Location;
    FVector    Velocity;
    float      RelativeTime;
    float      Lifetime;
    FVector    BaseVelocity;
    float      Rotation;
    float      RotationRate;
    FVector    Size;
    FColor     Color;
};

/*-----------------------------------------------------------------------------
    FParticleDataContainer
-----------------------------------------------------------------------------*/
struct FParticleDataContainer
{
    int32 MemBlockSize;
    int32 ParticleDataNumBytes;
    int32 ParticleIndicesNumShorts;
    uint8* ParticleData; // this is also the memory block we allocated
    uint16* ParticleIndices; // not allocated, this is at the end of the memory block

    FParticleDataContainer()
    : MemBlockSize(0)
    , ParticleDataNumBytes(0)
    , ParticleIndicesNumShorts(0)
    , ParticleData(nullptr)
    , ParticleIndices(nullptr)
    {
    }
    ~FParticleDataContainer()
    {
        Free();
    }
    void Alloc(int32 InParticleDataNumBytes, int32 InParticleIndicesNumShorts);
    void Free();
};

/*-----------------------------------------------------------------------------
    FDynamicEmitterReplayDataBase
-----------------------------------------------------------------------------*/
struct FDynamicEmitterReplayDataBase
{
    /** The type of emitter. */
    EDynamicEmitterType eEmitterType;

    /** The number of particles currently active in this emitter. */
    int32 ActiveParticleCount;

    int32 ParticleStride;
    FParticleDataContainer DataContainer;

    FVector Scale;

    int32 SortMode;
};

struct FDynamicSpriteEmitterReplayDataBase : public FDynamicEmitterReplayDataBase
{
    // FIXME : UMaterial로 변경.
    //UMaterial* MaterialInterface;
    FTexture* Texture;
    //struct UParticleModuleRequired* RequiredModule;
    FVector EmitterOrigin;
    
};

/** Source data for Sprite emitters */
struct FDynamicSpriteEmitterReplayData
    : public FDynamicSpriteEmitterReplayDataBase
{
    /** Constructor */
    FDynamicSpriteEmitterReplayData()
    {
    }
};

struct FDynamicEmitterDataBase
{
    int32 EmitterIndex;
    
    virtual const FDynamicEmitterReplayDataBase& GetSource() const = 0;

    virtual int const GetDynamicVertexStride() const = 0;
};

struct FDynamicSpriteEmitterDataBase : public FDynamicEmitterDataBase
{
    virtual const FDynamicSpriteEmitterReplayDataBase* GetSourceData() const = 0;
    void SortSpriteParticles();
    
    
    FDynamicEmitterReplayDataBase& GetSource();

    // 추상화된 기본 제공
    virtual const int32 GetDynamicVertexStride() const override
    {
        return sizeof(FParticleSpriteVertex);
    }
};

struct FDynamicSpriteEmitterData : public FDynamicSpriteEmitterDataBase
{
    FDynamicSpriteEmitterReplayData Source;

	/** Returns the source data for this particle system */
	/*virtual const FDynamicSpriteEmitterReplayData& GetSource() const override
	{
		return Source;
	}
    */
    virtual const FDynamicEmitterReplayDataBase& GetSource() const override
    {
        return Source;
    }

    virtual const FDynamicSpriteEmitterReplayDataBase* GetSourceData() const override
    {
        return &Source;
    }

    virtual const int32 GetDynamicVertexStride() const override
    {
        return sizeof(FParticleSpriteVertex);
    }
};

struct FDynamicMeshEmitterData : public FDynamicSpriteEmitterData
{
    /*virtual int32 GetDynamicVertexStride() const override
    {
        return sizeof(FMeshParticleInstanceVertex);
    }*/
};

#define DECLARE_PARTICLE(Name,Address)		\
	FBaseParticle& Name = *((FBaseParticle*) (Address));

#define DECLARE_PARTICLE_CONST(Name,Address)		\
	const FBaseParticle& Name = *((const FBaseParticle*) (Address));

#define DECLARE_PARTICLE_PTR(Name,Address)		\
	FBaseParticle* Name = (FBaseParticle*) (Address);