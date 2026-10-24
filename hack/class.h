#include <cstdint>

struct PlayerList {
    uintptr_t addr[60];
    int count;
};

    
struct PlayerList playerlist = {0};
int checkPlayer(void* player) {
    for (int i = 0; i < playerlist.count; i++) {
        if (playerlist.addr[i] == (uintptr_t)player) {
            return 0;
        }
    }
    return 1;
}
void addPlayer(void* player) {
    playerlist.addr[playerlist.count] = (uintptr_t)player;
    LOGI("Player Adde: 0x%lx", (uintptr_t)playerlist.addr[playerlist.count]);

    playerlist.count++;
}
void removePlayer(void* player) {
    for (int i = 0; i < playerlist.count; i++) {
        if (playerlist.addr[i] == (uintptr_t)player) {
            playerlist.addr[i] = playerlist.addr[playerlist.count - 1];
            LOGI("Player removed 0x%lx", playerlist.addr[i]);
            playerlist.count--;
        }
    }
}
void (*old_lateUpdate)(void* player);
void lateUpdate(void* player) {

    if (player) {
       int checkList = checkPlayer(player);

       if (checkList) {
           addPlayer(player);
       }
    }
    old_lateUpdate(player);
}
void (*old_onDestroy)(void* player);
void onDestroy(void* player) {
    if (player) {
        removePlayer(player);
    }

    old_onDestroy(player);
}

void test() {
    LOGI("Update Fucntion");
    uintptr_t playerUpdate = getAbsoluteAdress(RVA_playerUpdate);
    uintptr_t playerOnDestroy = getAbsoluteAdress(RVA_playerOnDestroy);
    int result1 = DobbyHook((void*)playerUpdate, (void *)lateUpdate, (void **)&old_lateUpdate);
    int result2 = DobbyHook((void*)playerOnDestroy, (void*)onDestroy, (void**)&old_onDestroy);
    LOGI("RESULT OF DOBBYHOOK Update: %d", result1);
    LOGI("RETURN OF DOBBYHOOK onDestroy: %d", result2);
}

