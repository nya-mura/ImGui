#include <cstdint>
#include <cstdio>
#include <cstring>
uintptr_t GetBaseAdress(const char* libname) {
    uintptr_t base = 0;
    char line[1024];
    FILE* file = fopen("/proc/self/maps", "r");
    if (!file) {
        return 0;
    }
    while (fgets(line, sizeof(line), file) != NULL) {

        if (strstr(line, libname) && strstr(line, "r-xp") != NULL) {
            break;
        }
        if (strstr(line, libname) && strstr(line, "r--p") != NULL) {
            sscanf(line, "%lx", &base);
        } 
    }
    fclose(file);
    return base;
}
