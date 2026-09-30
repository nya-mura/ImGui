#include <cstdint>      
bool line = false;


void DrawEsp(ImDrawList *drawlist, int screenWidth, int screenHeight) {
    void* BattleManager_Instance = nullptr;
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "BattleManager", "Instance", &BattleManager_Instance);
    if (BattleManager_Instance) {
        uintptr_t m_LocalPlayerShow = *(uintptr_t*)((uintptr_t)BattleManager_Instance + BattleManager_m_LocalPlayerShow);
        if (m_LocalPlayerShow) {
            Vector3 selfPos = *(Vector3*)((uintptr_t)m_LocalPlayerShow + ShowEntity__Position);
            Vector3 SelfPosW2S = WorldToScreen(selfPos);
            ImVec2 SelfPosVec2 = ImVec2(screenWidth - SelfPosW2S.x, SelfPosW2S.y);
            if (SelfPosW2S.z > 0) {
                SelfPosVec2 = ImVec2(SelfPosW2S.x, screenHeight - SelfPosW2S.y);
            }

            // Draws a solid red circle with a radius of 30.0f
            drawlist->AddCircleFilled(SelfPosVec2, 5, IM_COL32(255, 0, 0, 255));

        
        }
    }

}
