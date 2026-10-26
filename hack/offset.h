#include <cstdint>
uintptr_t RVA_get_touchCount = 0x42d7bfc;
uintptr_t RVA_GetTouch = 0x42d76c4;
uintptr_t RVA_playerUpdate = 0x1faa7b0; //Class Player Method updaye()
uintptr_t RVA_playerOnDestroy = 0x1fabd58;  //Class Player Method onDestroy()
uintptr_t RVA_get_transform = 0x428dac0;
uintptr_t RVA_get_position = 0x4299c00;
uintptr_t RVA_get_main = 0x42629cc;
uintptr_t RVA_WorldToScreenPoint = 0x4262658;
uintptr_t RVA_get_width = 0x426ba38;
uintptr_t RVA_get_height = 0x426ba60;

uintptr_t get_transform = getAbsoluteAdress(RVA_get_transform);
uintptr_t get_position = getAbsoluteAdress(RVA_get_position);
uintptr_t get_main = getAbsoluteAdress(RVA_get_main);
uintptr_t WorldToScreenPoint = getAbsoluteAdress(RVA_WorldToScreenPoint);
uintptr_t get_width = getAbsoluteAdress(RVA_get_width);
uintptr_t get_height = getAbsoluteAdress(RVA_get_height);

void* (*Transform)(void*) = (void*(*)(void*))get_transform;
Vector3 (*Position)(void*) = (Vector3(*)(void*))get_position;
void* (*Main)() = (void*(*)())get_main;
Vector3 (*World)(void*, Vector3) = (Vector3(*)(void*, Vector3))WorldToScreenPoint;
int (*Width)() = (int(*)())get_width;
int (*Height)() = (int(*)())get_height;


Vector3 GetPosition(void* player) {
    Vector3 pos = Position(Transform(player));
    return pos;
}


