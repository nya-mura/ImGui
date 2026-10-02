#include <cstdint>      
bool line = false;
typedef struct {
    float x;
    float y;
    float z;
} Vector3;

typedef struct {
    float x;
    float y;
} Vector2;

/*Class Screen*/
#define Screen_get_width (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Screen", "get_width")
#define Screen_get_height (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Screen", "get_height")
#define Screen_get_dpi (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Screen", "get_dpi")

int get_width() {
   int (*fn)() = (int(*)())Screen_get_width;
   return fn();
}
int get_height() {
    int (*fn)() = (int(*)())Screen_get_width;
    return fn();
}
int get_dpi() {
    int (*fn)() = (int(*)())Screen_get_dpi;
    return fn();
}
#define Transform_get_position (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Transform", "get_position")
#define Component_get_transform (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Component", "get_transform")
#define Camera_get_main (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_main")
#define Camera_WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "WorldToScreenPoint", 1)
#define Camera_get_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_fieldOfView")
#define Camera_set_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "set_fieldOfView", 1)
void* get_transform(void* instance) {
    void* (*fn)(void*) = (void*(*)(void*))Component_get_transform;
    return fn(instance);
}
Vector3 get_position(void* instance) {
    Vector3 (*fn)(void*) = (Vector3(*)(void*))Transform_get_position;
    return fn(instance);
}
void* get_main() {
    void* (*fn)() = (void*(*)())Camera_get_main;
    return fn();
}

Vector3 WorldToScreen(Vector3 position){
    Vector3 (*fn)(void*, Vector3) = (Vector3(*)(void*, Vector3))Camera_WorldToScreenPoint;
    return fn(get_main(), position);
}

// void *get_main() {
//   return reinterpret_cast<void *(__fastcall *)()>(Camera_get_main)();
// }
//
// Vector3 WorldToScreen(Vector3 position) {
//   return reinterpret_cast<Vector3(__fastcall *)(void *, Vector3)>(
//       Camera_WorldToScreenPoint)(get_main(), position);
// }
float get_fieldOfView() {
    float (*fn)(void*) = (float(*)(void*))Camera_get_fieldOfView;
    return fn(get_main());
}

void* set_fieldOfView(float value) {
    void* (*fn)(void*, float) = (void*(*)(void*, float))Camera_set_fieldOfView;
    return fn(get_main(), value);
}
//Class BattleBridge
#define BattleBridge_get_bStartBattle (uintptr_t) Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "BattleBridge", "get_bStartBattle", 0)


//Class BattleManager
#define BattleManager_m_LocalPlayerShow (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_LocalPlayerShow")
#define BattleManager_m_ShowPlayers (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_ShowPlayers")
#define BattleManager_m_ShowMonsters (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_ShowMonsters")


//Class ShowEntity
#define ShowEntity__Position (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_vCachePosition")
#define ShowEntity_m_RoleName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_RoleName")



//Class EntityBase
#define EntityBase_m_bSameCampType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_bSameCampType")
#define EntityBase_m_IsDeathProgress (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_IsDeathProgress")
#define EntityBase_m_bDeath (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_bDeath")
#define EntityBase_canSight (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "canSight")
#define EntityBase_m_Hp (uintptr_t)  Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_Hp")
#define EntityBase_m_HpMax (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_HpMax")
#define EntityBase_m_ID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_ID")
#define EntityBase_m_uGuid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_uGuid")



void DrawEsp(ImDrawList *drawlist, int width, int height) {
    float screenWidth = (float)width;
    float screenHeight = (float)height;
    void* BattleManager_Instance = nullptr;
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "BattleManager", "Instance", &BattleManager_Instance);
    if (BattleManager_Instance) {
        uintptr_t m_LocalPlayerShow = *(uintptr_t*)((uintptr_t)BattleManager_Instance + BattleManager_m_LocalPlayerShow);
        if (m_LocalPlayerShow) {
            Vector3 selfPos = *(Vector3*)((uintptr_t)m_LocalPlayerShow + ShowEntity__Position);
            Vector3 SelfPosW2S = WorldToScreen(selfPos);
            Vector2 SelfPosVec2 = {screenWidth - SelfPosW2S.x, SelfPosW2S.y};
            if (SelfPosW2S.z <= 0) {
                SelfPosVec2 = {SelfPosW2S.x, screenHeight - SelfPosW2S.y};
                
            }
            monoList<void**> *m_ShowPlayers = *(monoList<void**> **)((uintptr_t)BattleManager_Instance + BattleManager_m_ShowPlayers);
            if (m_ShowPlayers) {
                for (int i = 0; i < m_ShowPlayers->getSize(); i++) {
                    void* Pawn = m_ShowPlayers->getItems()[i];
                    if (!Pawn) {
                        continue;
                    }
                    bool m_bSameCampType = *(bool*)((uintptr_t)Pawn + EntityBase_m_bSameCampType);
                    if (m_bSameCampType) {
                        continue;
                    }
                    bool m_bDeath = *(bool*)((uintptr_t)Pawn + EntityBase_m_bDeath);
                    if (m_bDeath) {
                        continue;
                    }
                    Vector3 position = *(Vector3*)((uintptr_t)Pawn + ShowEntity__Position);
                    Vector3 RootPosW2S = WorldToScreen(position);
                    Vector2 RootPosVec2 = {screenWidth - RootPosW2S.x, RootPosW2S.y};
                    if (RootPosW2S.z > 0) {
                        RootPosVec2 = {RootPosW2S.x, screenHeight - RootPosW2S.y};
                    }

                    drawlist->AddCircleFilled(ImVec2(SelfPosVec2.x, SelfPosVec2.y), 5, IM_COL32(255, 0, 0, 255));
                    drawlist->AddLine(ImVec2(SelfPosVec2.x, SelfPosVec2.y), ImVec2(RootPosVec2.x, RootPosVec2.y), IM_COL32(205, 205, 205, 205), 1.7f);
                }
            }
            
        }
    

        
        
    }

}
