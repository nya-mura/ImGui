#include <jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <bits/pthread_types.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "Dobby/dobby.h"
#include "ByNameModding/Includes.h"
#include "ByNameModding/fake_dlfcn.h"
#include "Viscount/xdl/xdl.h"
#include <android/log.h>
#include "ByNameModding/obfuscate.h"
#include "ByNameModding/Il2Cpp.h"
#include "Viscount/extra.h"

uintptr_t base = 0;

bool debug = false;

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "IMGUI", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "IMGUI", __VA_ARGS__)
#define HOOKAF(ret, func, ...)      \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)

HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac)
{
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

#define libName "libil2cpp.so"

#include <pthread.h>
#include <jni.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include "Viscount/memory.h"
#include "hack/offset.h"
#include "hack/class.h"
#include "hack/esp.h"
#include "touch.h"

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSawpBuffer(EGLDisplay dpy, EGLSurface surface)
{
    //
    // static bool logged = false;
    //
    // if (!logged) {
    //     logged = true;
    //
    //     LOGI("eglSwapBuffers HOOK REACHED");
    //     LOGI("EGL context: %p", (void*)eglGetCurrentContext());
    //     LOGI("GL version: %s", glGetString(GL_VERSION));
    // }
    static bool g_Initialized = false;
    static bool should_clear_mouse_pos = false;
    if (!g_Initialized)
    {
        // LOGI("Creating ImGui context");
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = NULL;
        bool ok = ImGui_ImplOpenGL3_Init("#version 300 es");
        // LOGI("OpenGL3 backend initialized: %d", ok);
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(0.2f);
        g_Initialized = true;
    }

    static bool idk = false;
    static float value = 0.0f;
    static bool debug = false;
    EGLint w, h;
    eglQuerySurface(dpy, surface, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &h);

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);

    touch(&should_clear_mouse_pos);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // DrawEsp();

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Cakey");

    ImGui::Text("fwd");
    ImGui::Text("Base: 0x%lx", base);
    ImGui::Text("EntityList: %d", playerlist.count);
    ImGui::Checkbox("Esp Line", &line);
    ImGui::Checkbox("Debug", &debug);



    if (debug)
    {
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

    

        void* (*Transform)(void*) = (void*(*)(void*))get_transform;
        void (*Position)(void*, Vector3*) = (void(*)(void*, Vector3*))get_position_Injected;
        void (*World)(void*, Vector3*, int, Vector3*) = (void(*)(void*, Vector3*, int, Vector3*))WorldToScreenPoint_Injected;
        void* (*Camera)() = (void*(*)())get_MainCamera;
        int (*Width)() = (int(*)())get_width;
        int (*Height)() = (int(*)())get_height;
        float (*Health)(void*) = (float(*)(void*))get_health;
        int (*Team)(void*) = (int(*)(void*))get_TeamNumber;
        bool (*Local)(void*) = (bool(*)(void*))get_isLocalPlayer;
        void* (*Bone)(void*, int) = (void*(*)(void*, int))GetBoneTransform;
        int width = Width();
        int height = Height();
        ImGui::Text("Screen Width: %d", width);
        ImGui::Text("Screen Height: %d", height);
        int myTeamId = -1;
        for (int i = 0; i < playerlist.count; i++)
        {
            void *Player = (void *)playerlist.addr[i];
            if (Player == NULL)
            {
                ImGui::Text("Player NULL");
                continue;
            }
            bool localPlayer = Local(Player);
            if (localPlayer)
            {
                myTeamId = Team(Player);
            }
            float playerHealth = Health(Player);
            int playerTeam = Team(Player);
            ImGui::Text("Player [%d] %p", i, Player);
            void* playerTransform = Transform(Player);
            if (playerTransform == NULL) {
                ImGui::Text("Player Transform NULL");
                continue;
            }
            ImGui::Text("Transform [%d] %p", i, playerTransform);
            void* camera = Camera();
            if (camera == NULL) {
                ImGui::Text("Camera not found NULL");
                continue;
            }
            ImGui::Text("Camera %p", camera);
            Vector3 playerPos;
            Position(playerTransform, &playerPos);
            // ImGui::Text("Team: [%d] Player Pos: %2.f %2.f %2.f Health: %3.f", playerTeam, playerPos.x, playerPos.y, playerPos.z, playerHealth);
            Vector3 worldpos = {0};
            World(camera, &playerPos, 2, &worldpos);

            if (worldpos.z < 1.f)
            {
                continue;
            }
            ImVec2 point(worldpos.x, height - worldpos.y);

            if (myTeamId == playerTeam)
            {
                continue;
            }

            if (playerHealth == 0)
            {
                continue;
            }
            
            ImGui::GetBackgroundDrawList()->AddCircle(point, 1.0f, IM_COL32(255, 0, 0, 255), 16, 3.0f);
            void** something = (void**)((uintptr_t)Player + 268);
            void* animator = *something;
            if (animator == NULL)
            {
                ImGui::Text("Animator is NULL");
                continue;
            }
            ImGui::Text("Animator %p", animator);
        }
    }
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (should_clear_mouse_pos)
    {
        io.MousePos = ImVec2(-1, -1);
        should_clear_mouse_pos = false;
    }
    return orig_eglSwapBuffers(dpy, surface);
}
void *sylphy(void *)
{
    LOGI("On Thread");

    while ((base = GetBaseAdress(libName)) == 0)
    {
        LOGE("Not found");
        sleep(4);
    }
    LOGI("Found lib 0x%lx", base);

    void *egl = dlopen("libEGL.so", RTLD_NOW);
    if (!egl)
    {
        return NULL;
    }
    LOGI("egl address: %p", egl);

    void *swap = dlsym(egl, "eglSwapBuffers");
    if (!swap)
    {
        return NULL;
    }
    LOGI("eglSwapBuffers address: %p", swap);
    // void* handle_input = xdl_open("libinput.so", XDL_DEFAULT);
    // if (!handle_input) {
    //     return NULL;
    // }
    // LOGI("handle_input address: %p", handle_input);
    // void* input = xdl_sym(handle_input, "_ZN7android11MotionEvent8copyFromEPKS0_b", NULL);
    // if (!input) {
    //     return  NULL;
    // }
    // LOGI("input address: %p", input);
    LOGI("Done");

    int result = DobbyHook(swap, (void *)hook_eglSawpBuffer, (void **)&orig_eglSwapBuffers);
    // int result1 = DobbyHook(input, (void*)myInput, (void**)&origInput );
    LOGI("DobbyHook result: %d", result);
    // LOGI("DobbyHook result1: %d", result1);
    LOGI("Original function: %p", (void *)orig_eglSwapBuffers);
    // LOGI("Input Fucntion: %p", (void*)origInput);
    LOGI("Return Dobby Hook");

    test();
    return NULL;
}
__attribute__((constructor)) void lib_main()
{
    LOGI("Staeted main");
    pthread_t trixie;
    pthread_create(&trixie, NULL, sylphy, NULL);
}

extern "C" jint JNIEXPORT JNI_OnLoad(JavaVM *vm, void *key)
{
    JNIEnv *env;
    vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    return JNI_VERSION_1_6;
}
