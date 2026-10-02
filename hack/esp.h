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
            Vector3 selfPos = *(Vector3*)((uintptr_t)m_LocalPlayerShow + ShowEntity__Position);
            Vector3 SelfPosW2S = WorldToScreen(selfPos);
            Vector2 SelfPosVec2 = {screenWidth - SelfPosW2S.x, SelfPosW2S.y};
            if (SelfPosW2S.z <= 0) {
                SelfPosVec2 = {SelfPosW2S.x, screenHeight - SelfPosW2S.y};
                
            }
            monoList<void**> *m_ShowPlayers = *(monoList<void**> **)((uintptr_t)BattleManager_Instance + BattleManager_m_ShowPlayers);
                if (m_ShowPlayers) {
                    for (int i = 0; i < m_ShowPlayers->getSize(); i++) {
                        uintptr_t Pawn = m_ShowPlayers->getItems()[i];
                        if (!Pawn) {
                            continue;
                        }
                        bool m_bSameCampType = *(bool*)((uintptr_t)Pawn + m_bSameCampType);
                        if (m_bSameCampType) {
                            continue;
                        }
                        bool m_bDeath = *(bool*)((uintptr_t)Pawm + EntityBase_m_bDeath);
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
