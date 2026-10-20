#include <cstdint>

struct PlayerList {
    uintptr_t addr[60];
    int count;
};

    
struct PlayerList playerlist = {0};

void (*old_lateUpdate)(void* player);
void lateUpdate(void* player) {
    LOGI("update Reach");
    if (player) {
        for (int i = 0; i < playerlist.count; i++) {
            if (playerlist.addr[i] == (uintptr_t)player) {
                return;
            }
        }

        playerlist.addr[playerlist.count] = (uintptr_t)player;
        LOGI("Player Adde: 0x%lx", (uintptr_t)playerlist.addr[playerlist.count]);
        playerlist.count++;


    }
    old_lateUpdate(player);
}
void test() {

    LOGI("Test Fucntion");
    uintptr_t test = base + 0x1FAA7B0;
    LOGI("TEST : 0x%lx", test);
    int result = DobbyHook((void*)test, (void *)lateUpdate, (void **)&old_lateUpdate);
    LOGI("RESULT OF DOBBYHOOK: %d", result);
    

}
