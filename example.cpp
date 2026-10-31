int pairCount = sizeof(skeleton) / sizeof(skeleton[0]);

for (int i = 0; i < pairCount; i++)
{
    Vector3 worldA{}, worldB{};
    Vector3 screenA{}, screenB{};

    void *transformA = GetBoneTransform(animator, skeleton[i].from);
    void *transformB = GetBoneTransform(animator, skeleton[i].to);

    if (!transformA || !transformB)
        continue;

    Position(transformA, &worldA);
    Position(transformB, &worldB);

    World(camera, worldA, 2, &screenA);
    World(camera, worldB, 2, &screenB);

    if (screenA.z <= 0.0f || screenB.z <= 0.0f)
        continue;

    ImGui::GetBackgroundDrawList()->AddLine(
        ImVec2(screenA.x, screenHeight - screenA.y),
        ImVec2(screenB.x, screenHeight - screenB.y),
        IM_COL32(255, 255, 255, 255),
        2.0f);
}
Vector3 bonePositions[19];

Vector3 bonePositions[19] = {};

for (int i = 0; i < 19; i++)
{
    void *transform = GetBoneTransform(animator, i);

    if (transform != nullptr)
    {
        Position(transform, &bonePositions[i]);
    }
}

int pairCount = sizeof(skeleton) / sizeof(skeleton[0]);

for (int i = 0; i < pairCount; i++)
{
    int boneA = skeleton[i].from;
    int boneB = skeleton[i].to;

    Vector3 screenA = {};
    Vector3 screenB = {};

    World(camera, bonePositions[boneA], 2, &screenA);
    World(camera, bonePositions[boneB], 2, &screenB);

    if (screenA.z <= 0.0f || screenB.z <= 0.0f)
        continue;

    ImGui::GetBackgroundDrawList()->AddLine(
        ImVec2(screenA.x, screenHeight - screenA.y),
        ImVec2(screenB.x, screenHeight - screenB.y),
        IM_COL32(255, 255, 255, 255),
        2.0f);
}
Vector3 bonePositions[19] = {};
Vector3 screenPositions[19] = {};

for (int i = 0; i < pairCount; i++)
{
    int from = skeleton[i].from;
    int to = skeleton[i].to;

    Vector3 a = screenPositions[from];
    Vector3 b = screenPositions[to];

    if (a.z <= 0.0f || b.z <= 0.0f)
        continue;

    ImGui::GetBackgroundDrawList()->AddLine(
        ImVec2(a.x, screenHeight - a.y),
        ImVec2(b.x, screenHeight - b.y),
        IM_COL32(255, 255, 255, 255),
        2.0f);
}

if (playerHealth == 0)
{
    continue;
}

// Allocate a buffer for the C-string
char healthBuffer[16];

// Format the integer into the C-string buffer
snprintf(healthBuffer, sizeof(healthBuffer), "%d", playerHealth);

// Offset the position slightly to the right
ImVec2 textPos = ImVec2(point.x + 10.0f, point.y - 6.0f);

// Draw the C-string directly
ImGui::GetBackgroundDrawList()->AddText(textPos, IM_COL32(255, 0, 0, 255), healthBuffer);

This game already exposes the third - person skeleton directly — use Player + 0x270(ThirdPersonBodyPartReferences)instead : void *refs = *(void **)((uintptr_t)Player + 0x270);
if (refs)
{
    void *head = *(void **)((uintptr_t)refs + 0x20);
    void *spine = *(void **)((uintptr_t)refs + 0x28);
    void *hips = *(void **)((uintptr_t)refs + 0x30);
    void *leftUpperArm = *(void **)((uintptr_t)refs + 0x38);
    void *leftLowerArm = *(void **)((uintptr_t)refs + 0x40);
    void *rightUpperArm = *(void **)((uintptr_t)refs + 0x48);
    void *rightLowerArm = *(void **)((uintptr_t)refs + 0x50);
    void *leftUpperLeg = *(void **)((uintptr_t)refs + 0x58);
    void *leftLowerLeg = *(void **)((uintptr_t)refs + 0x60);
    void *rightUpperLeg = *(void **)((uintptr_t)refs + 0x68);
    void *rightLowerLeg = *(void **)((uintptr_t)refs + 0x70);
    void *transforms = *(void **)((uintptr_t)refs + 0x78); // Il2CppArray<Transform*>
}

void *refs = *(void **)((uintptr_t)Player + 0x270);

if (refs == NULL)
{
    continue;
}

void *bones[11] =
    {
        *(void **)((uintptr_t)refs + 0x20), // 0: Head
        *(void **)((uintptr_t)refs + 0x28), // 1: Spine
        *(void **)((uintptr_t)refs + 0x30), // 2: Hips
        *(void **)((uintptr_t)refs + 0x38), // 3: LeftUpperArm
        *(void **)((uintptr_t)refs + 0x40), // 4: LeftLowerArm
        *(void **)((uintptr_t)refs + 0x48), // 5: RightUpperArm
        *(void **)((uintptr_t)refs + 0x50), // 6: RightLowerArm
        *(void **)((uintptr_t)refs + 0x58), // 7: LeftUpperLeg
        *(void **)((uintptr_t)refs + 0x60), // 8: LeftLowerLeg
        *(void **)((uintptr_t)refs + 0x68), // 9: RightUpperLeg
        *(void **)((uintptr_t)refs + 0x70)  // 10: RightLowerLeg
};

Vector3 bonePos[11] = {};
Vector3 boneScreen[11] = {};
bool boneValid[11] = {};

for (int i = 0; i < 11; i++)
{
    if (bones[i] == NULL)
    {
        continue;
    }

    Position(bones[i], &bonePos[i]);

    World(camera, &bonePos[i], 2, &boneScreen[i]);

    if (boneScreen[i].z > 0.0f)
    {
        boneValid[i] = true;
    }
}

struct BonePair
{
    int from;
    int to;
};

const BonePair skeleton[] =
    {
        {0, 1}, // Head -> Spine
        {1, 2}, // Spine -> Hips

        {1, 3}, // Spine -> LeftUpperArm
        {3, 4}, // LeftUpperArm -> LeftLowerArm

        {1, 5}, // Spine -> RightUpperArm
        {5, 6}, // RightUpperArm -> RightLowerArm

        {2, 7}, // Hips -> LeftUpperLeg
        {7, 8}, // LeftUpperLeg -> LeftLowerLeg

        {2, 9}, // Hips -> RightUpperLeg
        {9, 10} // RightUpperLeg -> RightLowerLeg
};

const size_t pairCount = sizeof(skeleton) / sizeof(skeleton[0]);

for (size_t i = 0; i < pairCount; i++)
{
    int from = skeleton[i].from;
    int to = skeleton[i].to;

    if (!boneValid[from] || !boneValid[to])
    {
        continue;
    }

    ImGui::GetBackgroundDrawList()->AddLine(
        ImVec2(boneScreen[from].x, height - boneScreen[from].y),
        ImVec2(boneScreen[to].x, height - boneScreen[to].y),
        IM_COL32(255, 0, 255, 255),
        2.0f);
}

void *closestEnemy = NULL;
float closestDistanceSquared = FLT_MAX;

for (int i = 0; i < playerCount; i++)
{
    void *enemy = players[i];

    if (enemy == NULL)
        continue;

    Vector3 enemyWorld;
    GetPosition(enemy, &enemyWorld);

    float dx = enemyWorld.x - localWorld.x;
    float dy = enemyWorld.y - localWorld.y;
    float dz = enemyWorld.z - localWorld.z;

    float worldDistanceSquared =
        dx * dx + dy * dy + dz * dz;

    if (worldDistanceSquared < closestDistanceSquared)
    {
        closestDistanceSquared = worldDistanceSquared;
        closestEnemy = enemy;
    }
}

// Option A: Exact layout (48 bytes on 64-bit, 44 bytes on 32-bit)
struct CustomRaycastHit
{
    Vector3 point;    // 0x00 (12 bytes)
    Vector3 normal;   // 0x0C (12 bytes)
    uint32_t faceId;  // 0x18 (4 bytes)
    float distance;   // 0x1C (4 bytes)
    Vector2 uv;       // 0x20 (8 bytes - assuming Vector2 is 2 floats)
    int32_t collider; // 0x28 (4 bytes - Unity 2019+ uses instanceID int, older uses Collider*)
};

// Option B: Raw byte-padding (if you only want to pass the return value through)
struct RawRaycastHit
{
    uint8_t data[44]; // Adjust to 48 if m_Collider is a 64-bit pointer in your Unity version
};

#include "dobby.h"

// Function pointer to hold the original function
CustomRaycastHit (*orig_FireBullet)(
    void *damager,
    Vector3 origin,
    Vector3 direction,
    int weaponType,
    Vector3 visualOrigin,
    void *muzzle);

// Detour function
CustomRaycastHit hook_FireBullet(
    void *damager,
    Vector3 origin,
    Vector3 direction,
    int weaponType,
    Vector3 visualOrigin,
    void *muzzle)
{
    // Modify arguments before calling original (e.g., silent aim direction)
    // direction = GetTargetDirection(origin);

    // Call original function
    CustomRaycastHit hit = orig_FireBullet(damager, origin, direction, weaponType, visualOrigin, muzzle);

    // Or modify the returned struct data before returning to caller
    // hit.distance = 999.0f;

    return hit;
}

// Inside your initialization thread:
void InstallHook(void *targetAddress)
{
    DobbyHook(
        targetAddress,
        (void *)hook_FireBullet,
        (void **)&orig_FireBullet);
}

#include <math.h>

struct Vector3
{
    float x, y, z;
};

// Helper function to calculate normalized direction from Origin to Target
Vector3 GetSilentDirection(Vector3 origin, Vector3 targetPos)
{
    // 1. Subtract origin from target to get the directional vector
    Vector3 delta;
    delta.x = targetPos.x - origin.x;
    delta.y = targetPos.y - origin.y;
    delta.z = targetPos.z - origin.z;

    // 2. Calculate magnitude (distance between origin and target)
    float length = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

    // 3. Normalize the vector (make its length 1.0f)
    if (length > 0.0001f)
    {
        delta.x /= length;
        delta.y /= length;
        delta.z /= length;
    }

    return delta;
}

CustomRaycastHit hook_FireBullet(
    void *damager,
    Vector3 origin,
    Vector3 direction,
    int weaponType,
    Vector3 visualOrigin,
    void *muzzle)
{
    // Check if you have a valid enemy target
    if (hasValidEnemyTarget)
    {
        // Use the 'origin' already passed by the game!
        direction = GetSilentDirection(origin, enemyWorldPos);
    }

    // Pass the original 'origin' and your new normalized 'direction'
    return orig_FireBullet(damager, origin, direction, weaponType, visualOrigin, muzzle);
}

float fovRadius = 150.0f;
ImGui::SliderFloat("FOV Radius", &fovRadius, 10.0f, 500.0f, "%.0f px");

ImVec2 center(
    screenWidth / 2.0f,
    screenHeight / 2.0f);

ImGui::GetBackgroundDrawList()->AddCircle(
    center,
    fovRadius,
    IM_COL32(255, 255, 255, 200),
    64,
    1.5f);

float centerX = screenWidth / 2.0f;
float centerY = screenHeight / 2.0f;

float dx = screenPos.x - centerX;
float dy = screenPos.y - centerY;

float distance = sqrtf(dx * dx + dy * dy);

#include <cmath>

bool IsInsideCircle(
    float x,
    float y,
    float centerX,
    float centerY,
    float radius)
{
    float dx = x - centerX;
    float dy = y - centerY;

    return (dx * dx + dy * dy) <= (radius * radius);
}
