#pragma once
#include "Asset/Asset.h"
#include "D3D11RHI/GraphicDevice.h"
#include "container/String.h"
#include "HAL/PlatformType.h"

class UTexture : public UAsset
{
    DECLARE_CLASS(UTexture, UAsset)

    UTexture() = default;
    //UTexture(ID3D11ShaderResourceView* SRV, ID3D11Texture2D* Texture2D, uint32 _width, uint32 _height, FWString _path)
        //: TextureSRV(SRV), Texture(Texture2D), width(_width), height(_height), path(_path)
    //{}
    ~UTexture()
    {
		
    }
    void Release()
    {
        if (TextureSRV) { TextureSRV->Release(); TextureSRV = nullptr; }
        if (Texture) { Texture->Release(); Texture = nullptr; }
    }

    bool LoadFromFile(const FString& InFilepath) override;
private:
    bool LoadTextureFromDDS(const FString& InFilepath);
    bool LoadTextureFromFile(const FString& InFilepath);
    
public:
    bool SerializeToFile(std::ostream& Out) override;
    bool DeserializeFromFile(std::istream& In) override;

    ID3D11ShaderResourceView* TextureSRV = nullptr;
    ID3D11Texture2D* Texture = nullptr;
    FString Path;
    uint32 Width;
    uint32 Height;
};
