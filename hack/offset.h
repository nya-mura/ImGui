#include <cstdint>
uintptr_t RVA_get_touchCount = 0x42d7bfc;
uintptr_t RVA_GetTouch = 0x42d76c4;
uintptr_t RVA_playerUpdate = 0x1faa7b0;    // Class Player Method updaye()
uintptr_t RVA_playerOnDestroy = 0x1fabd58; // Class Player Method onDestroy()

void *(*Transform)(void *);
void (*Position)(void *, Vector3 *);
void (*World)(void *, Vector3 *, int, Vector3 *);
void* (*Camera)();
int (*Width)();
int (*Height)();
float (*Health)(void *);
int (*Team)(void *);
bool (*Local)(void *);
void *(*Bone)(void *, int);