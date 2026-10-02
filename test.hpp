#if defined(_MSC_VER) && defined(_M_IX86) // Windows x86 (32-bit)
    #define MY_FASTCALL __fastcall
#elif defined(__GNUC__) && defined(__i386__) // GCC/Clang x86 (32-bit)
    #define MY_FASTCALL __attribute__((fastcall))
#else
    #define MY_FASTCALL // Empty for ARM, Android, x64, etc.
#endif

#include <cstdint>
// #include "Modules/My/ToString.h"
bool esp = false;
bool line = false;
bool box = false;
bool health = false;
bool monster = false;
bool monhealth = false;


struct Vector2 {
        float x, y;

            Vector2() : x(0), y(0) {}

                Vector2(float x_, float y_) : x(x_), y(y_) {}
};
struct Vector3 {
        float x, y, z;

            Vector3() : x(0), y(0), z(0) {}

                Vector3(float x_, float y_, float z_)
                            : x(x_), y(y_), z(z_) {}
};
#define BattleBridge_bStartBattle (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleBridge", "bStartBattle")


#define Screen_get_width (uintptr_t)Il2CppGetMethodOffset("UnityEngine.CoreModule.dll", "UnityEngine", "Screen", "get_width", 0)
#define Screen_get_height (uintptr_t)Il2CppGetMethodOffset("UnityEngine.CoreModule.dll", "UnityEngine", "Screen", "get_height", 0)

#define Camera_get_main (uintptr_t)Il2CppGetMethodOffset("UnityEngine.CoreModule.dll", "UnityEngine", "Camera", "get_main")
#define Camera_WorldToScreenPoint (uintptr_t)Il2CppGetMethodOffset("UnityEngine.CoreModule.dll", "UnityEngine", "Camera", "WorldToScreenPoint", 1)

#define BattleManager_m_ShowPlayers (uintptr_t)Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_ShowPlayers")
#define BattleManager_m_LocalPlayerShow (uintptr_t)Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_LocalPlayerShow")
#define BattleManager_m_ShowMonsters (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_ShowMonsters")
#define ShowEntity_Position (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll",  "", "ShowEntity", "m_vCachePosition")
#define EntityBase_m_bDeath (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_bDeath")
#define EntityBase_m_bSameCampType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_bSameCampType")

#define EntityBase_m_ID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_ID")
#define EntityBase_canSight (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "canSight")
#define EntityBase_m_Hp (uintptr_t)  Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_Hp")
#define EntityBase_m_HpMax (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_HpMax")


void *get_main() {
  return reinterpret_cast<void *(__fastcall *)()>(Camera_get_main)();
}

Vector3 WorldToScreenPoint(Vector3 position) {
  return reinterpret_cast<Vector3(__fastcall *)(void *, Vector3)>(
      Camera_WorldToScreenPoint)(get_main(), position);
}

int get_screen_width() {
  uintptr_t methodAddr = Screen_get_width;
  if (methodAddr == 0)
    return (int)ImGui::GetIO().DisplaySize.x;
  return reinterpret_cast<int (*)()>(methodAddr)();
}
int get_screen_height() {
  uintptr_t methodAddr = Screen_get_height;
  if (methodAddr == 0)
    return (int)ImGui::GetIO().DisplaySize.y;
  return reinterpret_cast<int (*)()>(methodAddr)();
}



void DrawEsp(ImDrawList* draw) {
    float gameW = (float)get_screen_width();
    float gameH = (float)get_screen_height();
  if (!esp) {
    return;
  }
  void *BattleBridge_Instance;
  Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "BattleData",
                            "m_BattleBridge", &BattleBridge_Instance);
  if (!BattleBridge_Instance) {
    return;
  }

  void *BattleManager_Instance;
  Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "BattleManager", "Instance",
                            &BattleManager_Instance);
  if (!BattleManager_Instance) {
    return;
  }
  uintptr_t localPlayer = *(uintptr_t *)((uintptr_t)BattleManager_Instance + BattleManager_m_LocalPlayerShow);
  if (localPlayer) {

      Vector3 selfWorldPos = *(Vector3 *)((uintptr_t)localPlayer + ShowEntity_Position);
      selfWorldPos.y -= 3.6f;
      Vector3 selfScreenPos = WorldToScreenPoint(selfWorldPos);
      ImVec2 self = ImVec2(gameW - selfScreenPos.x, selfScreenPos.y);
      if (selfScreenPos.z > 0) {
          self = ImVec2(selfScreenPos.x, gameH - selfScreenPos.y);
      }
      monoList<void **> *m_ShowPlayers = *(monoList<void **> **)((uintptr_t)BattleManager_Instance + BattleManager_m_ShowPlayers);
      if (m_ShowPlayers) {
          int playerCount = m_ShowPlayers->getSize();
          for (int i = 0; i < playerCount; i++) {
              void* Pawn = m_ShowPlayers->getItems()[i];
              if (!Pawn) {
                  continue;
              }
              bool isDead = *(bool *)((uintptr_t)Pawn + EntityBase_m_bDeath);
              if (isDead) {
                  continue;
              }
              bool isTeammate = *(bool *)((uintptr_t)Pawn + EntityBase_m_bSameCampType);
              if (isTeammate) {
                  continue;
              }
              int curHP = *(int *)((uintptr_t)Pawn + EntityBase_m_Hp);
              int maxHP = *(int *)((uintptr_t)Pawn + EntityBase_m_HpMax);
              ImU32 lineColor = IM_COL32(34, 139,  34, 255);
              ImU32 boxColor = IM_COL32(127, 255, 212, 255);
              bool canSight = *(bool *)((uintptr_t)Pawn + EntityBase_canSight);
              if (canSight) {
                  lineColor = IM_COL32(220,  20,  60, 255);
                  boxColor = IM_COL32(138,  43, 226, 255);
              }
              Vector3 enemyWorldPos = *(Vector3 *) ((uintptr_t)Pawn + ShowEntity_Position);
              enemyWorldPos.y -= 3.6f;
              Vector3 enemyScreenPos = WorldToScreenPoint(enemyWorldPos);
              ImVec2 enemy = ImVec2(gameW - enemyScreenPos.x, enemyScreenPos.y);
              if (enemyScreenPos.z > 0) {
                  enemy = ImVec2(enemyScreenPos.x,gameH - enemyScreenPos.y);
              }
              Vector2 enemyHead = Vector2(enemy.x, enemy.y - (gameH / 10.35));
              if (line) {
                  draw->AddLine(self, enemy, lineColor, 1.7f);
                  draw->AddCircleFilled(self, 5, IM_COL32(255, 0, 0, 255));
                  draw->AddCircleFilled(enemy, 5, IM_COL32(0, 255, 0, 255));
              }
              if (box) {
                  float boxHeight = abs(enemyHead.y - enemy.y) * 1.75f;
                  float boxWidth = boxHeight * 0.75f;
                  ImVec2 vStart = {enemyHead.x - (boxWidth / 2), enemyHead.y};
                  ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                  draw->AddRect(vStart, vEnd, boxColor, 0, 140, 1.7f);
              }
              if (health) {
                  float boxHeight = abs(enemyHead.y - enemy.y) * 1.75f;
                  float boxWidth = boxHeight * 0.75f;
                  ImVec2 vStart = {enemyHead.x - (boxWidth / 2), enemyHead.y};
                  ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                  float hpPercent = (float)curHP / (float)maxHP;
                  float barWidth = 5.0f;
                  float gap = 3.0f;
                  ImVec2 hpBgStart = {vStart.x - barWidth - gap, vStart.y};
                  ImVec2 hpBgEnd = {vStart.x - gap, vEnd.y};
                  float fillHeight = boxHeight * hpPercent;
                  ImVec2 hpFillStart = {hpBgStart.x, hpBgEnd.y - fillHeight};
                  ImVec2 hpFillEnd = hpBgEnd;
                  draw->AddRectFilled(hpBgStart, hpBgEnd, IM_COL32(40, 40, 40, 220));
                  draw->AddRectFilled(hpFillStart, hpFillEnd, IM_COL32(220,  20,  60, 255));

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
              int m_ID = *(int *)((uintptr_t)Pawn + EntityBase_m_ID);
              // if (MonsterToString(m_ID) == "") {
              //     continue;
              // }
              // bool m_bSameCampType = *(bool *)((uintptr_t)Pawn + EntityBase_m_bSameCampType);
              // if (m_bSameCampType) {
              //     continue;
              // }
              bool isDead = *(bool *)((uintptr_t)Pawn + EntityBase_m_bDeath);
              if (isDead) {
                  continue;
              }
              int curHP = *(int *)((uintptr_t)Pawn + EntityBase_m_Hp);
              int maxHP = *(int *)((uintptr_t)Pawn + EntityBase_m_HpMax);
              ImU32 boxColor = IM_COL32(255, 215, 0, 255);
              bool canSight = *(bool *)((uintptr_t)Pawn + EntityBase_canSight);
              if (canSight) {
                  boxColor = IM_COL32(255, 165, 0, 255);
              }
              Vector3 creepWorldPos = *(Vector3 *)((uintptr_t)Pawn + ShowEntity_Position);
              creepWorldPos.y -= 3.6f;
              Vector3 creepScreenPos = WorldToScreenPoint(creepWorldPos);
              ImVec2 creep = ImVec2(gameW - creepScreenPos.x, creepScreenPos.y);
              if (creepScreenPos.z > 0) {
                  creep = ImVec2(creepScreenPos.x, gameH - creepScreenPos.y);
              }
              Vector2 creepHead = Vector2(creep.x, creep.y - (gameH / 10.35));
              if (monster) {
                  float boxHeight = abs(creepHead.y - creep.y) * 1.75f;
                  float boxWidth = boxHeight * 0.75;
                  ImVec2 vStart = {creepHead.x - (boxWidth / 2), creepHead.y};
                  ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                  draw->AddRect(vStart, vEnd, boxColor, 0, 240, 1.7f);
              }
              if (monhealth) {
                  float boxHeight = abs(creepHead.y - creep.y) * 1.75f;
                  float boxWidth = boxHeight * 0.75f;
                  ImVec2 vStart = {creepHead.x - (boxWidth / 2), creepHead.y};
                  ImVec2 vEnd = {vStart.x + boxWidth, vStart.y + boxHeight};
                  float hpPercent = (float)curHP / (float)maxHP;
                  float barWidth = 5.0f;
                  float gap = 3.0f;
                  ImVec2 hpBgStart = {vStart.x - barWidth - gap, vStart.y};
                  ImVec2 hpBgEnd = {vStart.x - gap, vEnd.y};
                  float fillHeight = boxHeight * hpPercent;
                  ImVec2 hpFillStart = {hpBgStart.x, hpBgEnd.y - fillHeight};
                  ImVec2 hpFillEnd = hpBgEnd;
                  draw->AddRectFilled(hpBgStart, hpBgEnd, IM_COL32(40, 40, 40, 220));
                  draw->AddRectFilled(hpFillStart, hpFillEnd, IM_COL32(220,  20,  60, 255));
              }
          }
      }

  }
}
