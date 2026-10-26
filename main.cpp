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
        // ImGui::GetStyle().ScaleAllSizes(1.0f);
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

    DrawEsp();

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Dear ImGui, Cakey");

    ImGui::Text("Test");
    ImGui::Text("Base: 0x%lx", base);
    ImGui::Text("EntityList: %d", playerlist.count);
    ImGui::Checkbox("Esp Line", &line);

    int width = Width();
    int height = Height();
    ImGui::Text("Width: %d", width);
    ImGui::Text("Height: %d", height);

    if (debug)
    {
        for (int i = 0; i < playerlist.count; i++)
        {
            void *Player = (void *)playerlist.addr[i];
            if (Player == NULL)
            {
                ImGui::Text("NULL");
                continue;
            }
            Vector3 playerPos = GetPosition(Player);
            Vector3 WorldPos = WorldToScreenPoint(playerPos);
            ImGui::Text("Player Pos: %d %d %d", playerPos.x, playerPos.y, playerPos.z);
            ImGui::Text("World Pos: %d %d %d", WorldPos.x, WorldPos.y, WorldPos.z);
            if (WorldPos.z < 1.f)
            {
                continue;
            }
            
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
