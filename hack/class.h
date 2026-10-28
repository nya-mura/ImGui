#include <cstdint>

struct PlayerList
{
    uintptr_t addr[60];
    int count;
};

struct PlayerList playerlist = {0};
int checkPlayer(void *player)
{
    for (int i = 0; i < playerlist.count; i++)
    {
        if (playerlist.addr[i] == (uintptr_t)player)
        {
            return 0;
        }
    }
    return 1;
}
void addPlayer(void *player)
{
    playerlist.addr[playerlist.count] = (uintptr_t)player;
    LOGI("Player Adde: 0x%lx", (uintptr_t)playerlist.addr[playerlist.count]);

    playerlist.count++;
}
void removePlayer(void *player)
{
    for (int i = 0; i < playerlist.count; i++)
    {
        if (playerlist.addr[i] == (uintptr_t)player)
        {
            playerlist.addr[i] = playerlist.addr[playerlist.count - 1];
            LOGI("Player removed 0x%lx", playerlist.addr[i]);
            playerlist.count--;
        }
    }
}
void (*old_lateUpdate)(void *player);
void lateUpdate(void *player)
{

    if (player)
    {
        int checkList = checkPlayer(player);

        if (checkList)
        {
            addPlayer(player);
        }
    }
    old_lateUpdate(player);
}
void (*old_onDestroy)(void *player);
void onDestroy(void *player)
{
    if (player)
    {
        removePlayer(player);
    }

    old_onDestroy(player);
}

void test()
{
    LOGI("Update Fucntion");
    uintptr_t playerUpdate = getAbsoluteAdress(RVA_playerUpdate);
    uintptr_t playerOnDestroy = getAbsoluteAdress(RVA_playerOnDestroy);
    int result1 = DobbyHook((void *)playerUpdate, (void *)lateUpdate, (void **)&old_lateUpdate);
    int result2 = DobbyHook((void *)playerOnDestroy, (void *)onDestroy, (void **)&old_onDestroy);
    LOGI("RESULT OF DOBBYHOOK Update: %d", result1);
    LOGI("RETURN OF DOBBYHOOK onDestroy: %d", result2);

    uintptr_t get_transform = base + 0x428dac0;
    uintptr_t get_position_Injected = base + 0x4299c5c;
    uintptr_t get_MainCamera = base + 0x1f17ccc;
    uintptr_t WorldToScreenPoint_Injected = base + 0x4262464;
    uintptr_t get_width = base + 0x426ba38;
    uintptr_t get_height = base + 0x426ba60;
    uintptr_t get_health = base + 0x1f95944;
    uintptr_t get_TeamNumber = base + 0x1f95e44;
    uintptr_t get_isLocalPlayer = base + 0x1f94b9c;
    uintptr_t GetBoneTransform = base + 0x42585e4;

    Transform = (void *(*)(void *))get_transform;
    Position = (void (*)(void *, Vector3 *))get_position_Injected;
    World = (void (*)(void *, Vector3 *, int, Vector3 *))WorldToScreenPoint_Injected;
    Camera = (void *(*)())get_MainCamera;
    Width = (int (*)())get_width;
    Height = (int (*)())get_height;
    Health = (float (*)(void *))get_health;
    Team = (int (*)(void *))get_TeamNumber;
    Local = (bool (*)(void *))get_isLocalPlayer;
    Bone = (void *(*)(void *, int))GetBoneTransform;
}
