#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <bits/pthread_types.h>
#include <cstddef>
#include <cstdint>
#include <dlfcn.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "Dobby/dobby.h"
#include "Viscount/Zygisk/zygisk.h"
#include "ByNameModding/Includes.h"
#include "ByNameModding/fake_dlfcn.h"
#include "ByNameModding/Il2Cpp.h"
#include "struct/MonoString.h"

#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MLBB", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "MLBB", __VA_ARGS__)



#include <pthread.h>
#include <jni.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include "Viscount/memory.h"
#include "Viscount/mine.h"
#include "ByNameModding/Unity.h"
#include "hack/ToString.h"
#include "hack/class.h"
#include "hack/esp.h"
// #include "test.hpp"
//
#define targetPackageName "com.mobiin.gp:UnityKillsMe"


using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

uintptr_t base = 0;

bool clearMousePos = true, setup = false;
struct UnityEngine_Vector2_Fields {
    float x;
    float y;
};

struct UnityEngine_Vector2_o {
    UnityEngine_Vector2_Fields fields;
};

enum TouchPhase {
    Began = 0,
    Moved = 1,
    Stationary = 2,
    Ended = 3,
    Canceled = 4
};




struct UnityEngine_Touch_Fields {
    int32_t m_FingerId;
    struct UnityEngine_Vector2_o m_Position;
    struct UnityEngine_Vector2_o m_RawPosition;
    struct UnityEngine_Vector2_o m_PositionDelta;
    float m_TimeDelta;
    int32_t m_TapCount;
    int32_t m_Phase;
    int32_t m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_fRadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};

void touch(bool* mouse) {
    ImGuiIO& io = ImGui::GetIO();
    int (*TouchCount)(void*) = (int (*)(void*)) (Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "get_touchCount", 0));
    int touchCount = TouchCount(nullptr);
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields (*)(int)) (Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "GetTouch", 1))) (0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;

        switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                io.MouseDown[0] = true;
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io.MouseDown[0] = false;
                *mouse = true;
                break;
            case TouchPhase::Moved:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                break;
            default:
                break;
        }
    } else {
        io.MouseDown[0] = false;
    }
}

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSawpBuffer(EGLDisplay dpy, EGLSurface surface) {
    static bool first = true;

        if (first) {
            first = false;
            LOGI("eglSwapBuffers HOOK CALLED");
        }
        
    static bool g_Initialized = false;
    static bool should_clear_mouse_pos = false;
    if (!g_Initialized) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        ImGui_ImplOpenGL3_Init("#version 300 es");
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(2.0f);
        g_Initialized = true;
    }

    static bool idk = false;
    static float value = 0.0f;
    static bool debug = false;
    EGLint w, h;
    eglQuerySurface(dpy, surface, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &h);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);
    touch(&should_clear_mouse_pos);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    DrawEsp(ImGui::GetBackgroundDrawList(), get_width(), get_height());
    // DrawEsp(ImGui::GetBackgroundDrawList());
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Dear ImGui");
       ImGui::Checkbox("Debug Menu", &debug);
       ImGui::Text("Test");
    ImGui::End(); 
    if (debug) {
        ImGui::Begin("Debug");
        ImGui::Text("Debug information");
        ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Base Adress: 0x%lx\n", base);
        void* BattleManager_Instance = nullptr;

        }
        ImGui::End();
    
    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (should_clear_mouse_pos) {
        io.MousePos = ImVec2(-1, -1);
        should_clear_mouse_pos = false;
    }
    return orig_eglSwapBuffers(dpy, surface);


}

void *sylphy(const char*) {
    LOGI("sylphy started");
    LOGI("sylphy waiting for liblogic.so");
    while ((base = GetBaseAdress("liblogic.so")) == 0) {
        LOGI("liblogic.so not found");
        sleep(3);
    }


    LOGI("liblogic.so found: %p", (void*)base);

    Il2CppAttach("liblogic.so");

    LOGI("Il2CppAttach done");
    sleep(10);
    void *egl = dlopen("libEGL.so", RTLD_NOW);
    if (!egl) {
        return nullptr;
    }
    void *swap = dlsym(egl, "eglSwapBuffers");
    if (!swap) {
        LOGE("dlsym eglSwapBuffers failed: %s", dlerror());
        return nullptr;
    }
    LOGI("eglSwapBuffers=%p", swap);
    DobbyHook(swap, (void*)hook_eglSawpBuffer, (void**)&orig_eglSwapBuffers);
    LOGI("DobbyHook returned");
    return nullptr;
}
// __attribute__((constructor))
// void lib_main() {
//     pthread_t trixie;
//     pthread_create(&trixie, NULL, sylphy, NULL);
//
// }


class ImGuiModMenu : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;

        LOGI("onLoad");
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        auto package_name = env->GetStringUTFChars(args->nice_name, nullptr);
        auto app_data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);

        LOGI("preAppSpecialize package=%s", package_name);
        preSpecialize(package_name, app_data_dir);
        LOGI("enable_hack=%d", enable_hack);
        env->ReleaseStringUTFChars(args->nice_name, package_name);
        env->ReleaseStringUTFChars(args->app_data_dir, app_data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        LOGI("postAppSpecialize enable_hack=%d", enable_hack);
        if (enable_hack) {
            LOGI("starting hack thread");
            std::thread hack_thread(sylphy, game_data_dir);
            hack_thread.detach();
        }
    }
	

private:
    Api *api;
    JNIEnv *env;
    bool enable_hack;
    char *game_data_dir;

    void preSpecialize(const char *package_name, const char *app_data_dir) {
        if (strcmp(package_name, targetPackageName) == 0) {
            enable_hack = true;
            game_data_dir = new char[strlen(app_data_dir) + 1];
            strcpy(game_data_dir, app_data_dir);
        }
    }
};

REGISTER_ZYGISK_MODULE(ImGuiModMenu)
