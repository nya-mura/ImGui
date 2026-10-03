#include <cstdint>      
#include <sys/mman.h>
bool line = true;
bool box = true;
bool health = true;
bool name = true;
bool monBox = true;
bool monHealth = true;
bool monName = true;


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
            if (SelfPosW2S.z > 0) {
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
                    bool canSight = *(bool*)((uintptr_t)Pawn + EntityBase_canSight);
                    int CurHp = *(int*)((uintptr_t)Pawn + EntityBase_m_Hp);
                    int MaxHp = *(int*)((uintptr_t)Pawn + EntityBase_m_HpMax);

                    Vector3 position = *(Vector3*)((uintptr_t)Pawn + ShowEntity__Position);
                    MonoString* m_HeroName = *(MonoString **)((uintptr_t)Pawn + ShowPlayer_m_HeroName);
                    Vector3 RootPosW2S = WorldToScreen(position);
                    Vector2 RootPosVec2 = {screenWidth - RootPosW2S.x, RootPosW2S.y};
                    if (RootPosW2S.z > 0) {
                        RootPosVec2 = {RootPosW2S.x, screenHeight - RootPosW2S.y};
                    }
                    ImU32 lineColor = IM_COL32(34, 139,  34, 255);
                    ImU32 boxColor = IM_COL32(127, 255, 212, 255);
                    if (canSight) {
                        lineColor = IM_COL32(220,  20,  60, 255);
                        boxColor = IM_COL32(138,  43, 226, 255);
                    }

                    Vector2 HeadPosVec2 = {RootPosVec2.x, (float)(RootPosVec2.y - (screenHeight / 10.35))};

                    drawlist->AddCircleFilled(ImVec2(SelfPosVec2.x, SelfPosVec2.y), 5, IM_COL32(255, 0, 0, 255));
                    if (line) {                    
                        drawlist->AddLine(ImVec2(SelfPosVec2.x, SelfPosVec2.y), ImVec2(RootPosVec2.x, RootPosVec2.y), IM_COL32(205, 205, 205, 205), 1.7f);
                    }
		            if (box) {
                        float boxHeight = abs(HeadPosVec2.y - RootPosVec2.y);
                        float boxWidth = boxHeight * 0.75f;
                        ImVec2 vStart = {HeadPosVec2.x - (boxWidth / 2), HeadPosVec2.y};
                        ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                        drawlist->AddRect(vStart, vEnd, boxColor, 0, 140, 1.7f);
                    }

                    if (health) {
                        float boxHeight = abs(HeadPosVec2.y - RootPosVec2.y);
                        float boxWidth = boxHeight * 0.75f;
                        ImVec2 vStart = {HeadPosVec2.x - (boxWidth / 2), HeadPosVec2.y};
                        ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                        float hpPercent = (float)CurHp / (float)MaxHp;
                        float barWidth = 5.0f;
                        float gap = 3.0f;
                        ImVec2 hpBgStart = {vStart.x - barWidth - gap, vStart.y};
                        ImVec2 hpBgEnd = {vStart.x - gap, vEnd.y};
                        float fillHeight = boxHeight * hpPercent;
                        ImVec2 hpFillStart = {hpBgStart.x, hpBgEnd.y - fillHeight};
                        ImVec2 hpFillEnd = hpBgEnd;
                        drawlist->AddRectFilled(hpBgStart, hpBgEnd, IM_COL32(40, 40, 40, 220));
                        drawlist->AddRectFilled(hpFillStart, hpFillEnd, IM_COL32(220, 20, 60, 255));
                    }
                    if (name) {
                        if (m_HeroName) {
                            char strName[64];
                            snpritf("[ %s ]", m_HeroName.toString());
                            ImVec2 textSize = ImGui::CalcTextSize(strName, 0, ((float) screenHeight / 39.0f));
                            drawlist->AddText(NULL, ((float) screenHeight / 39.0f), {RootPosVec2.x - (textSize.x / 2), RootPosVec2.y + 25}, IM_COL32(255, 255, 255, 255), strName);

                        }
                    }
                }
            }
            monoList<void **> *m_ShowMonsters = *(monoList<void **> **)((uintptr_t)BattleManager_Instance + BattleManager_m_ShowMonsters);
            if (m_ShowMonsters) {
                for (int i = 0; i < m_ShowMonsters->getSize(); i++) {
                    void* Pawn = m_ShowMonsters->getItems()[i];
                    if (!Pawn) {
                        continue;
                    }
                    int m_ID = *(int*)((uintptr_t)Pawn + EntityBase_m_ID);
                    const char* strName = monsterToString(m_ID);
                    if (strcmp(strName, "NO") == 0) {
                        continue;

                    }
                    
                    bool m_bDeath = *(bool*)((uintptr_t)Pawn + EntityBase_m_bDeath);
                    if (m_bDeath) {
                        continue;
                    }
                    bool canSight = *(bool*)((uintptr_t)Pawn + EntityBase_canSight);
                    int CurHp = *(int*)((uintptr_t)Pawn + EntityBase_m_Hp);
                    int MaxHp = *(int*)((uintptr_t)Pawn + EntityBase_m_HpMax);
                    Vector3 position = *(Vector3*)((uintptr_t)Pawn + ShowEntity__Position);
                    Vector3 RootPosW2S = WorldToScreen(position);
                    Vector2 RootPosVec2 = {screenWidth - RootPosW2S.x, RootPosW2S.y};
                    if (RootPosW2S.z > 0) {
                        RootPosVec2 = {RootPosW2S.x, screenHeight - RootPosW2S.y};
                    }
                    ImU32 boxColor = IM_COL32(255, 215, 0, 255);
                    if (canSight) {
                        boxColor = IM_COL32(255, 165, 0, 255);
                    }
                    Vector2 HeadPosVec2 = {RootPosVec2.x, (float)(RootPosVec2.y - (screenHeight / 10.35))};
                    if (monBox) {

                        float boxHeight = abs(HeadPosVec2.y - RootPosVec2.y);
                        float boxWidth = boxHeight * 0.75;
                        ImVec2 vStart = {HeadPosVec2.x - (boxWidth / 2), HeadPosVec2.y};
                        ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                        drawlist->AddRect(vStart, vEnd, boxColor, 0, 240, 1.7f);
                    }
                    if (monHealth) {

                        float boxHeight = abs(HeadPosVec2.y - RootPosVec2.y);
                        float boxWidth = boxHeight * 0.75f;
                        ImVec2 vStart = {HeadPosVec2.x - (boxWidth / 2), HeadPosVec2.y};
                        ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                        float hpPercent = (float)CurHp / (float)MaxHp;
                        float barWidth = 5.0f;
                        float gap = 3.0f;
                        ImVec2 hpBgStart = {vStart.x - barWidth - gap, vStart.y};
                        ImVec2 hpBgEnd = {vStart.x - gap, vEnd.y};
                        float fillHeight = boxHeight * hpPercent;
                        ImVec2 hpFillStart = {hpBgStart.x, hpBgEnd.y - fillHeight};
                        ImVec2 hpFillEnd = hpBgEnd;
                        drawlist->AddRectFilled(hpBgStart, hpBgEnd, IM_COL32(40, 40, 40, 220));
                        drawlist->AddRectFilled(hpFillStart, hpFillEnd, IM_COL32(220,  20,  60, 255));
                    }

                    if (monName) {
                        ImVec2 textSize = ImGui::CalcTextSize(strName, 0, ((float) screenHeight / 39.0f));
                        drawlist->AddText(NULL, ((float) screenHeight / 39.0f), {RootPosVec2.x - (textSize.x / 2), RootPosVec2.y + 25}, IM_COL32(255, 255, 100, 255), strName);
                    }

                    if (m_ID == 20022 && CurHp < MaxHp) {
                        const char* strAlert = "ALERT!!! Lord is under attack!";
                        ImVec2 textSize = ImGui::CalcTextSize(strAlert, 0, ((float)screenHeight / 31.0f));
                        drawlist->AddText(NULL, ((float) screenHeight / 31.0f), {(float)(screenWidth / 2) - (textSize.x / 2), (float)(screenHeight - screenHeight) + (float)(screenHeight / 8.7f)}, IM_COL32(255, 255, 100, 255), strAlert);
                    }

                    if (m_ID == 2003 && CurHp < MaxHp) {
                        const char* strAlert = "ALERT!!! Turtle is under attack!";
                        ImVec2 textSize = ImGui::CalcTextSize(strAlert, 0, ((float)screenHeight / 31.0f));
                        drawlist->AddText(NULL, ((float) screenHeight / 31.0f), {(float)(screenWidth / 2) - (textSize.x / 2), (float)(screenHeight - screenHeight) + (float)(screenHeight / 8.7f)}, IM_COL32(255, 255, 100, 255), strAlert);
                    }
                }
            }
        }    
    }

}
