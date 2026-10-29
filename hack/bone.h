
struct BonePair
{
    int from;
    int to;
};

struct BonePair skeleton[] =
{
    {0, 1},   // Head -> Spine
    {1, 2},   // Spine -> Hips

    {1, 3},   // Spine -> LeftUpperArm
    {3, 4},   // LeftUpperArm -> LeftLowerArm

    {1, 5},   // Spine -> RightUpperArm
    {5, 6},   // RightUpperArm -> RightLowerArm

    {2, 7},   // Hips -> LeftUpperLeg
    {7, 8},   // LeftUpperLeg -> LeftLowerLeg

    {2, 9},   // Hips -> RightUpperLeg
    {9, 10}   // RightUpperLeg -> RightLowerLeg
};

size_t pairCount = sizeof(skeleton) / sizeof(skeleton[0]);


// struct BonePair {
//     int from;
//     int to;
// };

// struct BonePair skeleton[] = {
//     {10, 9},  // Head -> Neck
//     {9, 8},   // Neck -> Chest
//     {8, 7},   // Chest -> Spine
//     {7, 0},   // Spine -> Hips
//     {8, 11},  // Chest -> LeftShoulder
//     {11, 13}, // LeftShoulder -> LeftUpperArm
//     {13, 15}, // LeftUpperArm -> LeftLowerArm
//     {15, 17}, // LeftLowerArm -> LeftHand
//     {8, 12},  // Chest -> RightShoulder
//     {12, 14}, // RightShoulder -> RightUpperArm
//     {14, 16}, // RightUpperArm -> RightLowerArm
//     {16, 18}, // RightLowerArm -> RightHand
//     {0, 1},   // Hips -> LeftUpperLeg
//     {1, 3},   // LeftUpperLeg -> LeftLowerLeg
//     {3, 5},   // LeftLowerLeg -> LeftFoot
//     {0, 2},   // Hips -> RightUpperLeg
//     {2, 4},   // RightUpperLeg -> RightLowerLeg
//     {4, 6}    // RightLowerLeg -> RightFoot
// };

// size_t pairCount = sizeof(skeleton) / sizeof(skeleton[0]);

