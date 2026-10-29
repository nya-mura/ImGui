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
bool bone;
bool debug = false;
bool test1 = false;
int myTeamId = -1;
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
#include "hack/bone.h"
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
    ImGui::Checkbox("Test", &test1);
    ImGui::Checkbox("Debug", &debug);

    if (debug)
    {

        int width = Width();
        int height = Height();
        ImGui::Text("Screen Width: %d", width);
        ImGui::Text("Screen Height: %d", height);

        ImGui::Text("Team id %d", myTeamId);
        for (int i = 0; i < playerlist.count; i++)
        {
            void *Player = (void *)playerlist.addr[i];
            if (Player == NULL)
            {
                ImGui::Text("Player NULL");
                continue;
            }
            int playerTeam = Team(Player);
            bool isMine = *(bool *)((uintptr_t)Player + 0x22c);
            if (isMine)
            {
                ImGui::Text("isMine %p Team %d", Player, playerTeam);
            }

            bool localPlayer = Local(Player);
            if (localPlayer)
            {
                ImGui::Text("local %p", Player);
                myTeamId = Team(Player);
            }
            float playerHealth = Health(Player);

            ImGui::Text("Player [%d] %p Team", i, Player, playerTeam);
            void *playerTransform = Transform(Player);
            if (playerTransform == NULL)
            {
                ImGui::Text("Player Transform NULL");
                continue;
            }
            // ImGui::Text("Transform [%d] %p", i, playerTransform);
            void *camera = Camera();
            if (camera == NULL)
            {
                ImGui::Text("Camera not found NULL");
                continue;
            }
            // ImGui::Text("Camera %p", camera);
            Vector3 playerPos;
            Position(playerTransform, &playerPos);
            // ImGui::Text("Team: [%d] Player Pos: %2.f %2.f %2.f Health: %3.f", playerTeam, playerPos.x, playerPos.y, playerPos.z, playerHealth);
            Vector3 worldpos = {0};
            World(camera, &playerPos, 2, &worldpos);

            if (worldpos.z <= 0.0f)
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

            char healthBuffer[32];
            snprintf(healthBuffer, sizeof(healthBuffer), "Health %3.f", playerHealth);
            ImVec2 textPos(point.x + 10.0f, point.y - 6.0f);
            ImGui::GetBackgroundDrawList()->AddText(textPos, IM_COL32(255, 0, 0, 255), healthBuffer);

            void *ThirdPersonBodyPartReferences = *(void **)((uintptr_t)Player + 0x270);
            if (ThirdPersonBodyPartReferences == NULL)
            {
                continue;
            }
            void *bones[11] =
                {
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x20), // 0: Head
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x28), // 1: Spine
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x30), // 2: Hips
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x38), // 3: LeftUpperArm
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x40), // 4: LeftLowerArm
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x48), // 5: RightUpperArm
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x50), // 6: RightLowerArm
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x58), // 7: LeftUpperLeg
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x60), // 8: LeftLowerLeg
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x68), // 9: RightUpperLeg
                    *(void **)((uintptr_t)ThirdPersonBodyPartReferences + 0x70)  // 10: RightLowerLeg
            };

            Vector3 bonePos[11] = {};
            Vector3 boneWorld[11] = {};

            for (int i = 0; i < 11; i++)
            {
                if (bones[i] == NULL)
                {
                    ImGui::Text("Empty Bones %d", i);
                    continue;
                }
                // ImGui::Text("Bone found %d", i);
                Position(bones[i], &bonePos[i]);
                World(camera, &bonePos[i], 2, &boneWorld[i]);
            }

            for (int i = 0; i < pairCount; i++)
            {
                int from = skeleton[i].from;
                int to = skeleton[i].to;

                Vector3 a = boneWorld[from];
                Vector3 b = boneWorld[to];

                if (a.z <= 0.0f || b.z <= 0.0f)
                {
                    continue;
                }

                ImGui::GetBackgroundDrawList()->AddLine(
                    ImVec2(a.x, height - a.y),
                    ImVec2(b.x, height - b.y),
                    IM_COL32(255, 0, 255, 255),
                    2.0f);
            }
            // ImGui::GetBackgroundDrawList()->AddCircle(point, 1.0f, IM_COL32(255, 0, 0, 255), 16, 3.0f);
            // void *ThirdPersonAnimatorHelper = *(void **)((uintptr_t)Player + 0x260);
            // if (ThirdPersonAnimatorHelper == NULL)
            // {
            //     continue;
            // }
            // void *animator = *(void **)((uintptr_t)ThirdPersonAnimatorHelper + 0x38);

            // // void *animator = *(void **)((uintptr_t)Player + 268);

            // if (animator == NULL)
            // {
            //     ImGui::Text("Animator is NULL");
            //     LOGI("Animator is NULL");
            //     continue;
            // }
            // ImGui::Text("Animator %p", animator);
            // LOGI("Animator %p", animator);
            // Vector3 bonePosition[19] = {};
            // Vector3 boneWorld[19] = {};

            // {
            //     for (int i = 0; i < 54; i++)
            //     {
            //         void *boneTransform = Bone(animator, i);

            //         if (boneTransform == NULL)
            //         {
            //             ImGui::Text("BoneTransform NULL");
            //             LOGI("BoneTransform NULL for %d", i);
            //             continue;
            //         }
            //         ImGui::Text("BoneTransform %p", boneTransform);
            //         LOGI("BoneTransform %p for %d", boneTransform, i);
            //         Position(boneTransform, &bonePosition[i]);
            //         World(camera, &bonePosition[i], 2, &boneWorld[i]);
            //     }
            //     for (int i = 0; i < pairCount; i++)
            //     {
            //         int from = skeleton[i].from;
            //         int to = skeleton[i].to;

            //         Vector3 a = boneWorld[from];
            //         Vector3 b = boneWorld[to];

            //         if (a.z <= 0.0f || b.z <= 0.0f)
            //         {
            //             continue;
            //         }
            //         ImGui::GetBackgroundDrawList()->AddLine(
            //             ImVec2(a.x, height - a.y),
            //             ImVec2(b.x, height - b.y),
            //             IM_COL32(255, 0, 255, 255),
            //             2.0f);
            //    }
            // }
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
