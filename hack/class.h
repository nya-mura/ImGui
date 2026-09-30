
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

#define Camera_get_main (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_main")
#define Camera_WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "WorldToScreenPoint", 1)
#define Camera_get_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_fieldOfView")
#define Camera_set_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "set_fieldOfView", 1)

void* get_main() {
    void* (*fn)() = (void*(*)())Camera_get_main;
    return fn();
}

Vector3 WorldToScreen(Vector3 position){
    Vector3 (*fn)(void*, Vector3) = (Vector3(*)(void*, Vector3))Camera_WorldToScreenPoint;
    return fn(get_main(), position);
}
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

