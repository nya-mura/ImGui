#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <bits/pthread_types.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "Dobby/dobby.h"
#include "ByNameModding/Includes.h"
#include "ByNameModding/fake_dlfcn.h"
#include "ByNameModding/Il2Cpp.h"
#include <android/log.h>



#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "IMGUI", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "IMGUI", __VA_ARGS__)




#include <pthread.h>
#include <jni.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include "Viscount/memory.h"

static bool IsSafeMethodPtr(void* ptr) {
    return ptr != nullptr && (uintptr_t)ptr > 0x1000;
}

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
    void* touchCountPtr = Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "get_touchCount", 0);
    if (!IsSafeMethodPtr(touchCountPtr)) {
        io.MouseDown[0] = false;
        return;
    }

    int (*TouchCount)(void*) = (int (*)(void*)) (touchCountPtr);
    int touchCount = TouchCount(nullptr);
    if (touchCount > 0 && touchCount < 16) {
        void* getTouchPtr = Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "GetTouch", 1);
        if (!IsSafeMethodPtr(getTouchPtr)) {
            io.MouseDown[0] = false;
            return;
        }

        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields (*)(int)) (getTouchPtr)) (0);
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
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Dear ImGui");
    ImGui::Text("Android!");
    ImGui::Checkbox("Click", &idk);
    ImGui::SliderFloat("Value",&value,0.0f,100.0f);
    ImGui::Text("Bye!");
    ImGui::Checkbox("Debug Menu", &debug);
    ImGui::End(); 
    if (debug) {
        ImGui::Begin("Debug");
        ImGui::Text("Debug information");
        ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }
    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (should_clear_mouse_pos) {
        io.MousePos = ImVec2(-1, -1);
        should_clear_mouse_pos = false;
    }
    return orig_eglSwapBuffers(dpy, surface);

}
void *sylphy(void*) {
    LOGI("On Thread");

    uintptr_t base = 0;
    while ((base = GetBaseAdress("libil2cpp.so")) == 0) {
        LOGE("Not found");
        sleep(4);
    }
    LOGI("Found lib 0x%lx", base);


    Il2CppAttach("libil2cpp.so");
    sleep(60);
    LOGI("Attach");
    void *egl = dlopen("libEGL.so", RTLD_NOW);
    if (!egl) {
        return nullptr;
    }
   void *swap = dlsym(egl, "eglSwapBuffers");
    if (!swap) {
        return nullptr;
    }
    LOGI("Done");
    DobbyHook(swap, (void*)hook_eglSawpBuffer, (void**)&orig_eglSwapBuffers); 
    LOGI("Return Dobby Hook");
    return nullptr;
}
__attribute__((constructor))
void lib_main() {
    LOGI("Staeted main");
    pthread_t trixie;
    pthread_create(&trixie, NULL, sylphy, NULL);
    
}
