#include <cstdint>
uintptr_t RVA_get_touchCount = 0x42d7bfc;
uintptr_t RVA_GetTouch = 0x42d76c4;
uintptr_t RVA_playerUpdate = 0x1faa7b0;    // Class Player Method updaye()
uintptr_t RVA_playerOnDestroy = 0x1fabd58; // Class Player Method onDestroy()

void *(*Transform)(void *) = (void *(*)(void *))get_transform;
void (*Position)(void *, Vector3 *) = (void (*)(void *, Vector3 *))get_position_Injected;
void (*World)(void *, Vector3 *, int, Vector3 *) = (void (*)(void *, Vector3 *, int, Vector3 *))WorldToScreenPoint_Injected;
void *(*Camera)() = (void *(*)())get_MainCamera;
int (*Width)() = (int (*)())get_width;
int (*Height)() = (int (*)())get_height;
float (*Health)(void *) = (float (*)(void *))get_health;
int (*Team)(void *) = (int (*)(void *))get_TeamNumber;
bool (*Local)(void *) = (bool (*)(void *))get_isLocalPlayer;
void *(*Bone)(void *, int) = (void *(*)(void *, int))GetBoneTransform;
