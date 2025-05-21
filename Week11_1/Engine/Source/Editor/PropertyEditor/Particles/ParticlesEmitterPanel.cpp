#include "ParticlesEmitterPanel.h"
#include "CoreUObject/UObject/Casts.h"
#include "UnrealEd/ParticlePreviewUI.h"

#include "Classes/Particles/ParticleEmitter.h"
#include "Classes/Particles/Modules/ParticleModuleColor.h"
#include "Classes/Particles/Modules/ParticleModuleLifetime.h"
#include "Classes/Particles/Modules/ParticleModuleLocation.h"
#include "Classes/Particles/Modules/ParticleModuleRequired.h"
#include "Classes/Particles/Modules/ParticleModuleSize.h"
#include "Classes/Particles/Modules/ParticleModuleSpawn.h"
#include "Classes/Particles/Modules/ParticleModuleVelocity.h"
#include "Classes/Particles/TypeData/ParticleModuleTypeDataBase.h"
#include "Classes/Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Classes/Particles/ParticleLODLevel.h"
#include "Engine/FLoaderOBJ.h"

#include "Engine/Texture.h"

void ParticlesEmitterPanel::Initialize(SLevelEditor* LevelEditor, float InWidth, float InHeight)
{
    activeLevelEditor = LevelEditor;
    Width = InWidth;
    Height = InHeight;

    UClass* ModuleClass = UClass::FindClass("UParticleModule");
    GetChildOfClass(ModuleClass, ModuleClasses);
    ModuleClasses.Remove(UClass::FindClass("UParticleModule"));
    ModuleClasses.Remove(UClass::FindClass("UParticleModuleRequired"));
    ModuleClasses.Remove(UClass::FindClass("UParticleModuleSpawn"));
}

//void ParticlesEmitterPanel::Render()
//{
//    /* Pre Setup */
//    ImGuiIO& io = ImGui::GetIO();
//
//    const float PanelWidth = (Width) * (1 - UI->PreviewScreenWidth);
//    const float PanelHeight = (Height) * (1 - UI->PreviewScreenHeight) - 20.0f;
//
//    const float PanelPosX = Width - PanelWidth;
//    const float PanelPosY = 20.0f;
//
//    /* Panel Position */
//    ImGui::SetNextWindowPos(ImVec2(PanelPosX, PanelPosY), ImGuiCond_Always);
//
//    /* Panel Size */
//    ImGui::SetNextWindowSize(ImVec2(PanelWidth, PanelHeight), ImGuiCond_Always);
//
//    ImGui::Begin("Emitters Panel", nullptr);
//
//    // 예시: 여러 Emitter
//    // UParticleEmitter로 변경할것
//    const TArray<UParticleEmitter*>& Emitters = UI->GetSelectedSystem()->Emitters;
//
//    // Module을 정렬
//    struct EmitterModulesSorted {
//        UParticleEmitter* Emitter;
//        TArray<UParticleModule*> ModulesSorted;
//    };
//
//    TArray<EmitterModulesSorted> EmitterModulesSortedArray;
//
//    static const int RequiredIndex = 0;
//    static const int SpawnIndex = 1;
//
//    // COLUMNS
//    for (UParticleEmitter* Emitter : Emitters)
//    {
//        // 1. Module 정렬
//        TArray<UParticleModule*> Modules = Emitter->LODLevels[UI->GetSelectedLODIndex()]->Modules;
//
//        // Required 찾기
//        // iterate중에 바꿔도 되나??
//        for (int idx = RequiredIndex; idx < Modules.Num(); ++idx)
//        {
//            UParticleModule* Module = Modules[idx];
//            if (Module && Module->IsA<UParticleModuleRequired>())
//            {
//                if (idx != RequiredIndex)
//                {
//                    UParticleModule* Temp = Modules[RequiredIndex];
//                    Modules[RequiredIndex] = Module;
//                    Modules[idx] = Temp;
//                }
//                break;
//            }
//        }
//
//        // Spawn 찾기
//        for (int idx = SpawnIndex; idx < Modules.Num(); ++idx)
//        {
//            UParticleModule* Module = Modules[idx];
//            if (Module && Module->IsA<UParticleModuleSpawn>())
//            {
//                if (idx != SpawnIndex)
//                {
//                    UParticleModule* Temp = Modules[SpawnIndex];
//                    Modules[SpawnIndex] = Module;
//                    Modules[idx] = Temp;
//                }
//                break;
//            }
//        }
//
//        EmitterModulesSortedArray.Add({ Emitter, Modules });
//    }
//
//    // 3. 테이블 시작
//    if (EmitterModulesSortedArray.Num() > 0 && ImGui::BeginTable("EmitterModules", EmitterModulesSortedArray.Num()+1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
//    {
//        int MaxNumModules = 0;
//        for (const EmitterModulesSorted& EmitterModules : EmitterModulesSortedArray)
//        {
//            MaxNumModules = FMath::Max(MaxNumModules, EmitterModules.ModulesSorted.Num());
//        }
//
//        for (const EmitterModulesSorted& EmitterModules : EmitterModulesSortedArray)
//        {
//            const UParticleEmitter* Emitter = EmitterModules.Emitter;
//            ImGui::TableSetupColumn((*Emitter->GetName()));
//        }
//
//        ImGui::TableSetupColumn("Emitter 추가");
//
//        ImGui::TableHeadersRow();
//
//        // + 버튼을 위해서 최대개수보다 하나 더 추가
//        for (int row = 0; row < MaxNumModules + 1; ++row)
//        {
//            ImGui::TableNextRow();
//            for (int col = 0; col < EmitterModulesSortedArray.Num(); ++col)
//            {
//                ImGui::TableSetColumnIndex(col);
//                UParticleEmitter* Emitter = EmitterModulesSortedArray[col].Emitter;
//                if (row < EmitterModulesSortedArray[col].ModulesSorted.Num())
//                {
//                    UParticleModule* Module = EmitterModulesSortedArray[col].ModulesSorted[row];
//                    RenderModuleCell(Module, Emitter);
//                }
//                else if (row == EmitterModulesSortedArray[col].ModulesSorted.Num())
//                {
//                    RenderModuleAdd(Emitter);
//                }
//                else
//                {
//                    RenderModuleCell(nullptr, nullptr);
//                }
//            }
//
//            // emitter 추가 버튼
//            if (row == 0)
//            {
//                RenderEmitterAdd();
//            }
//        }
//
//
//
//        ImGui::EndTable();
//    }
//    ImGui::End();
//}
//
//void ParticlesEmitterPanel::RenderModuleCell(UParticleModule* Module, UParticleEmitter* Emitter) const
//{
//    if (!Module) return;
//    
//    bool Checked = UI->IsEnabled(Module);
//    FString ModuleName = Module->GetClass()->GetName();
//    ModuleName.RemoveFromStart(TEXT("UParticleModule"));
//    ImGui::Checkbox(*FString::Printf("##%d", Module->GetUUID()), &Checked);
//    // 체크했을때의 action은 없음
//    UI->SetEnabled(Module, Checked);
//
//    ImGui::SameLine();
//    
//    if (ImGui::Button(*FString::Printf("%s##%d", *ModuleName, Module->GetUUID())))
//    {
//        UI->SetSelectedModule(Module);
//    }
//
//    // 버튼 우클릭시 제거
//    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
//    {
//        if (!Module->IsA<UParticleModuleRequired>() && !Module->IsA<UParticleModuleSpawn>())
//        {
//            for (int i = 0; i < Emitter->LODLevels.Num(); ++i)
//            {
//                Emitter->LODLevels[i]->Modules.Remove(Module);
//                Emitter->LODLevels[i]->SpawnModules.Remove(Module);
//                Emitter->LODLevels[i]->UpdateModules.Remove(Module);
//            }
//            UI->RemoveFlags(Module);
//            GUObjectArray.MarkRemoveObject(Module);
//        }
//    }
//}
//
//void ParticlesEmitterPanel::RenderModuleAdd(UParticleEmitter* Emitter)
//{
//    uint32 Identifier = Emitter->GetUUID();
//
//    // Popup ID 고정 문자열 생성
//    FString PopupIdStr = FString::Printf(TEXT("AddModulePopup##%u"), Identifier);
//    const char* PopupId = *PopupIdStr;
//
//    // 버튼
//    if (ImGui::Button(*FString::Printf(TEXT("Add Module##%u_AddEmitter"), Identifier)))
//    {
//        ImGui::OpenPopup(PopupId);
//    }
//
//    // 팝업 시작
//    if (ImGui::BeginPopup(PopupId))
//    {
//        for (UClass* Child : ModuleClasses)
//        {
//            if (ImGui::MenuItem(*Child->GetName()))
//            {
//                // 모든 LOD level에 추가
//                for (int l = 0; l < Emitter->LODLevels.Num(); ++l)
//                {
//                    UParticleModule* NewModule = Cast<UParticleModule>(FObjectFactory::ConstructObject(Child, Emitter));
//                    Emitter->LODLevels[l]->Modules.Add(NewModule);
//                    UI->RegisterFlags(NewModule);
//                }
//                ImGui::Close Popup();
//            }
//        }
//
//        ImGui::EndPopup();
//    }
//}
//
//void ParticlesEmitterPanel::RenderEmitterAdd()
//{
//    if(ImGui::Button("+ Emitter"))
//    {
//        UParticleSystem* ParticleSystem = UI->GetSelectedSystem();
//
//        UParticleEmitter* NewEmitter = FObjectFactory::ConstructObject<UParticleEmitter>(ParticleSystem);
//
//        UParticleLODLevel* NewLODLevel = FObjectFactory::ConstructObject<UParticleLODLevel>(NewEmitter);
//        UParticleModuleRequired* RequiredModule = FObjectFactory::ConstructObject<UParticleModuleRequired>(NewLODLevel);
//        NewLODLevel->RequiredModule = RequiredModule;
//        NewLODLevel->Modules.Add(RequiredModule);
//
//        UParticleModuleSpawn* SpawnModule = FObjectFactory::ConstructObject<UParticleModuleSpawn>(NewLODLevel);
//        NewLODLevel->Modules.Add(SpawnModule);
//        NewLODLevel->SpawnModules.Add(SpawnModule);
//        NewLODLevel->AnalyzeModules();
//    }
//}

void ParticlesEmitterPanel::Render()
{
    ImGui::Begin("Emitters Panel", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

    const TArray<UParticleEmitter*>& Emitters = UI->GetSelectedSystem()->Emitters;

    for (int i = 0; i < Emitters.Num(); ++i)
    {
        UParticleEmitter* Emitter = Emitters[i];

        ImGui::PushID(Emitter->GetUUID()); // Emitter별로 고유 ID 스택 push

        FString ChildName = FString::Printf(TEXT("EmitterGroup_%d_%llu"), i, Emitter->GetUUID());
        ImGui::BeginChild((*ChildName), ImVec2(200, 0), true);

        RenderEmitter(Emitter);

        ImGui::EndChild();

        ImGui::PopID(); // Emitter별로 고유 ID 스택 pop

        if (i < Emitters.Num() - 1)
            ImGui::SameLine();
    }

    RenderEmitterAddButton();

    ImGui::End();
}

void ParticlesEmitterPanel::RenderEmitter(UParticleEmitter* Emitter)
{
    //ImGui::BeginGroup();

    // 왼쪽 150px : 이름, 체크박스, 솔로 버튼
    FString EmitterName = Emitter->EmitterName.ToString();
    
    ImGui::BeginChild(*FString("EmitterName" + EmitterName), ImVec2(110, 80), true);
    // 이름
    ImGui::Text("%s", *EmitterName);

    // 체크박스
    bool EmitterEnabled = UI->IsEnabled(Emitter);
    if (ImGui::Checkbox("##Enable", &EmitterEnabled))
    {
        UI->SetEnabled(Emitter, EmitterEnabled);
    }
    ImGui::SameLine();
    // 솔로 버튼
    bool EmitterSolo = UI->IsSolo(Emitter);
    if (ImGui::Checkbox("##Solo", &EmitterSolo))
    {
        UI->SetSolo(Emitter, EmitterSolo);
    }
    ImGui::SameLine();
    // 최대개수
    if (UParticleSpriteEmitter* EmitterSprite = Cast<UParticleSpriteEmitter>(Emitter))
    {
        int MaxNumParticles = EmitterSprite->MaxNumParticles;
        ImGui::Text("%d", MaxNumParticles);
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("EmitterTexture", ImVec2(80, 80), true);
    if (ID3D11ShaderResourceView* SRV = Emitter->LODLevels[0]->RequiredModule->Texture->TextureSRV)
    {
        ImTextureID LightDepth = reinterpret_cast<ImTextureID>(SRV);
        ImVec2 imageSize(50, 50); // 필요에 따라 크기 조정
        ImGui::Image(LightDepth, imageSize);
    }
    ImGui::EndChild();

    UParticleLODLevel* LODLevel = Emitter->LODLevels[UI->GetSelectedLODIndex()];

    TArray<UParticleModule*> Modules = Emitter->LODLevels[UI->GetSelectedLODIndex()]->Modules;

    int TypeDataIndex = 0;
    int RequiredIndex = 0;
    int SpawnIndex = 1;

    // Required 찾기
    for (int idx = RequiredIndex; idx < Modules.Num(); ++idx)
    {
        UParticleModule* Module = Modules[idx];
        if (Module && Module->IsA<UParticleModuleRequired>())
        {
            if (idx != RequiredIndex)
            {
                UParticleModule* Temp = Modules[RequiredIndex];
                Modules[RequiredIndex] = Module;
                Modules[idx] = Temp;
            }
            break;
        }
    }

    // Spawn 찾기
    for (int idx = SpawnIndex; idx < Modules.Num(); ++idx)
    {
        UParticleModule* Module = Modules[idx];
        if (Module && Module->IsA<UParticleModuleSpawn>())
        {
            if (idx != SpawnIndex)
            {
                UParticleModule* Temp = Modules[SpawnIndex];
                Modules[SpawnIndex] = Module;
                Modules[idx] = Temp;
            }
            break;
        }
    }
    RenderModuleTypeData(LODLevel->TypeDataModule, Emitter);
    for (UParticleModule* Module : Modules)
    {
        RenderModule(Module, Emitter);
    }

    RenderModuleAddButton(Emitter);
}

void ParticlesEmitterPanel::RenderModuleTypeData(UParticleModuleTypeDataBase* Module, UParticleEmitter* Emitter)
{
    if (Module)
    {
        FString ModuleUUID = FString::Printf("%llu", Module->GetUUID());
        ImGui::BeginChild(*ModuleUUID, ImVec2(0, 50), true);

        FString ModuleName = Module->GetClass()->GetName();
        ModuleName.RemoveFromStart(TEXT("UParticleModule"));
        // 체크했을때의 action은 없음

        if (ImGui::Button(*FString::Printf("%s##%d", *ModuleName, Module->GetUUID()), ImVec2(170, 35)))
        {
            UI->SetSelectedModule(Module);
        }

        // 버튼 우클릭시 제거
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            for (int i = 0; i < Emitter->LODLevels.Num(); ++i)
            {
                Emitter->LODLevels[i]->TypeDataModule = nullptr;
            }
            UI->RemoveFlags(Module);
            GUObjectArray.MarkRemoveObject(Module);
        }
        ImGui::EndChild();
        return;
    }
    else
    {
        ImGui::BeginChild(*FString::Printf("%llu", Emitter->GetUUID()), ImVec2(0, 50), true);

        if (ImGui::Button(*FString::Printf("Add Mesh##%d", Emitter->GetUUID()), ImVec2(170, 35)))
        {
            UParticleSystem* ParticleSystem = UI->GetSelectedSystem();
            
            UParticleModuleTypeDataMesh* NewMesh = FObjectFactory::ConstructObject<UParticleModuleTypeDataMesh>(nullptr);
            Emitter->LODLevels[UI->GetSelectedLODIndex()]->TypeDataModule = NewMesh;
            
            NewMesh->Mesh = FManagerOBJ::GetStaticMesh(L"apple_mid.obj");

            //NewLODLevel->AnalyzeModules();

            UI->RegisterFlags(NewMesh);
        }
        ImGui::EndChild();
    }
}

void ParticlesEmitterPanel::RenderModule(UParticleModule* Module, UParticleEmitter* Emitter)
{
    FString ModuleUUID = FString::Printf("%llu", Module->GetUUID());
    if (!Module) return;
    ImGui::PushID(Module->GetUUID()); // 모듈별로 고유 ID

    ImGui::BeginChild(*ModuleUUID, ImVec2(0, 50), true);

    FString ModuleName = Module->GetClass()->GetName();
    ModuleName.RemoveFromStart(TEXT("UParticleModule"));
    // 체크했을때의 action은 없음
    
    if (ImGui::Button(*FString::Printf("%s##%d", *ModuleName, Module->GetUUID()), ImVec2(170, 35)))
    {
        UI->SetSelectedModule(Module);
    }

    // 버튼 우클릭시 제거
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        if (!Module->IsA<UParticleModuleRequired>() 
            && !Module->IsA<UParticleModuleSpawn>()
            && !Module->IsA<UParticleModuleTypeDataBase>())
        {
            for (int i = 0; i < Emitter->LODLevels.Num(); ++i)
            {
                Emitter->LODLevels[i]->Modules.Remove(Module);
                Emitter->LODLevels[i]->SpawnModules.Remove(Module);
                Emitter->LODLevels[i]->UpdateModules.Remove(Module);
            }
            UI->RemoveFlags(Module);
            GUObjectArray.MarkRemoveObject(Module);
        }
    }

    ImGui::SameLine();

    //bool Checked = UI->IsEnabled(Module);
    //ImGui::Checkbox(*FString::Printf("##%d", Module->GetUUID()), &Checked);
    //UI->SetEnabled(Module, Checked);

    ImGui::EndChild();

    ImGui::PopID();

    return;
    //if (!Module) return;

    //ImGui::PushID(Module->GetUUID());

    //bool Checked = UI->IsEnabled(Module);
    //FString ModuleName = Module->GetClass()->GetName();
    //ModuleName.RemoveFromStart(TEXT("UParticleModule"));

    //ImGui::Checkbox("##Enable", &Checked);
    //UI->SetEnabled(Module, Checked);

    //ImGui::SameLine();
    //if (ImGui::Button((*ModuleName)))
    //{
    //    UI->SetSelectedModule(Module);
    //}

    //// 우클릭시 모듈 제거
    //if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    //{
    //    if (!Module->IsA<UParticleModuleRequired>() && !Module->IsA<UParticleModuleSpawn>())
    //    {
    //        for (int i = 0; i < Emitter->LODLevels.Num(); ++i)
    //        {
    //            Emitter->LODLevels[i]->Modules.Remove(Module);
    //            Emitter->LODLevels[i]->SpawnModules.Remove(Module);
    //            Emitter->LODLevels[i]->UpdateModules.Remove(Module);
    //        }
    //        UI->RemoveFlags(Module);
    //        GUObjectArray.MarkRemoveObject(Module);
    //    }
    //}
    //ImGui::PopID();
}

void ParticlesEmitterPanel::RenderModuleAddButton(UParticleEmitter* Emitter)
{
    FString PopupIdStr = FString::Printf(TEXT("AddModulePopup##%u"), Emitter->GetUUID());
    const char* PopupId = *PopupIdStr;

    if (ImGui::Button("Add Module"))
    {
        ImGui::OpenPopup(PopupId);
    }

    if (ImGui::BeginPopup(PopupId))
    {
        for (UClass* Child : ModuleClasses)
        {
            if (ImGui::MenuItem((*Child->GetName())))
            {
                for (int l = 0; l < Emitter->LODLevels.Num(); ++l)
                {
                    UParticleModule* NewModule = Cast<UParticleModule>(FObjectFactory::ConstructObject(Child, Emitter));
                    Emitter->LODLevels[l]->Modules.Add(NewModule);
                    UI->RegisterFlags(NewModule);
                }
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
}

void ParticlesEmitterPanel::RenderEmitterAddButton()
{

    ImGui::PushID("##EmitterAdd");
    ImGui::BeginChild("EmitterAddButton", ImVec2(100, 0), true);

    if (ImGui::Button("Add Emitter", ImVec2(90, 300)))
    {
        UParticleSystem* ParticleSystem = UI->GetSelectedSystem();
        UParticleEmitter* NewEmitter = FObjectFactory::ConstructObject<UParticleSpriteEmitter>(ParticleSystem);
        ParticleSystem->Emitters.Add(NewEmitter);

        UParticleLODLevel* NewLODLevel = FObjectFactory::ConstructObject<UParticleLODLevel>(NewEmitter);
        NewEmitter->LODLevels.Add(NewLODLevel);

        UParticleModuleRequired* RequiredModule = FObjectFactory::ConstructObject<UParticleModuleRequired>(NewLODLevel);
        NewLODLevel->RequiredModule = RequiredModule;
        NewLODLevel->Modules.Add(RequiredModule);

        UParticleModuleSpawn* SpawnModule = FObjectFactory::ConstructObject<UParticleModuleSpawn>(NewLODLevel);
        NewLODLevel->Modules.Add(SpawnModule);
        NewLODLevel->SpawnModules.Add(SpawnModule);
        NewLODLevel->AnalyzeModules();

        for (auto& lodLevel : NewEmitter->LODLevels)
        {
            for (auto& Module : lodLevel->Modules)
            {
                Module->InitializeDefaults();
            }
        }

        UI->Component->Deactivate();
        UI->Component->Activate();
        UI->RegisterFlags(ParticleSystem);
    }
    ImGui::EndChild();
    ImGui::PopID();
}


void ParticlesEmitterPanel::OnResize(HWND hWnd)
{
    RECT ClientRect;
    GetClientRect(hWnd, &ClientRect);
    Width = static_cast<float>(ClientRect.right - ClientRect.left);
    Height = static_cast<float>(ClientRect.bottom - ClientRect.top);
}