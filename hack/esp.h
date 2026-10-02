#include <cstdint>      
bool line = false;


void DrawEsp(ImDrawList *drawlist, int width, int height) {
    float screenWidth = (float)width;
    float screenHeight = (float)height;
    void* BattleManager_Instance = nullptr;
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "BattleManager", "Instance", &BattleManager_Instance);
    if (BattleManager_Instance) {
        uintptr_t m_LocalPlayerShow = *(uintptr_t*)((uintptr_t)BattleManager_Instance + BattleManager_m_LocalPlayerShow);
        if (m_LocalPlayerShow) {
            // Vector3 selfPos = get_position(get_transform((void*)m_LocalPlayerShow));
            Vector3 selfPos = *(Vector3*)((uintptr_t)m_LocalPlayerShow + ShowEntity__Position);
            Vector3 SelfPosW2S = WorldToScreen(selfPos);
            ImVec2 SelfPosVec2 = ImVec2(screenWidth - SelfPosW2S.x, SelfPosW2S.y);
            if (SelfPosW2S.z <= 0) {
                SelfPosVec2 = ImVec2(SelfPosW2S.x, screenHeight - SelfPosW2S.y);
                
            }
            // monoList<void**> *m_ShowPlayers = *(monoList<void**> **)((uintptr_t)BattleManager_Instance + BattleManager_m_ShowPlayers) {
            //     if (m_ShowPlayers) {
            //         for (int i = 0; i < m_ShowPlayers->getSize(); i++) {
            //             uintptr_t Pawn = m_ShowPlayers->getItems()[i];
            //             if (!Pawn) {
            //                 continue;
            //             }
            //             bool m_bSameCampType = *(bool*)((uintptr_t)Pawn + m_bSameCampType);
            //             if (m_bSameCampType) {
            //                 continue;
            //             }
            //             bool m_bDeath = *(bool*)((uintptr_t)Pawm + EntityBase_m_bDeath)
            //         }
            //     }
            // }
            //
            // Test render at fixed center of the screen
            //drawlist->AddCircleFilled(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), 20.0f, IM_COL32(0, 255, 0, 255));
            // Draws a solid red circle with a radius of 30.0f
            drawlist->AddCircleFilled(SelfPosVec2, 5, IM_COL32(255, 0, 0, 255));

        
        }
    }

}
