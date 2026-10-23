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

    int result1 = DobbyHook((void*)playerUpdate, (void *)lateUpdate, (void **)&old_lateUpdate);
    LOGI("RESULT OF DOBBYHOOK Update: %d", result1);
    LOGI("Old Update addr Fucntion %p", (void*)old_lateUpdate);
}

