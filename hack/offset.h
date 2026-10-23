#include <cstdint>
uintptr_t get_touchCount = getAbsoluteAdress(0x42d7bfc);
uintptr_t GetTouch = getAbsoluteAdress(0x42d76c4); 
uintptr_t playerUpdate = getAbsoluteAdress(0x1faa7b0); //Class Player Method updaye()
uintptr_t playerOnDestroy = getAbsoluteAdress(0x1fabd58);  //Class Player Method onDestroy()
uintptr_t get_transform = getAbsoluteAdress(0x428dac0);
uintptr_t get_position = getAbsoluteAdress(0x4299c00);
uintptr_t get_main = getAbsoluteAdress(0x42629cc);
uintptr_t WorldToScreenPoint = getAbsoluteAdress(0x4262658);
void* (*Transform)(void*) = (void*(*)(void*))get_transform;
Vector3 (*Position)(void*) = (Vector3(*)(void*))get_position;
void* (*Main)() = (void*(*)())get_main;
Vector3 (*World)(void*, Vector3) = (Vector3(*)(void*, Vector3))WorldToScreenPoint;

Vector3 GetPosition(void* player) {
    Vector3 pos = Position(Transform(player));
    return pos;
}


