struct BonePair {
    int from;
    int to;
};

struct BonePair skeleton[] = {
    {10, 9},  // Head -> Neck
    {9, 8},   // Neck -> Chest
    {8, 7},   // Chest -> Spine
    {7, 0},   // Spine -> Hips
    {8, 11},  // Chest -> LeftShoulder
    {11, 13}, // LeftShoulder -> LeftUpperArm
    {13, 15}, // LeftUpperArm -> LeftLowerArm
    {15, 17}, // LeftLowerArm -> LeftHand
    {8, 12},  // Chest -> RightShoulder
    {12, 14}, // RightShoulder -> RightUpperArm
    {14, 16}, // RightUpperArm -> RightLowerArm
    {16, 18}, // RightLowerArm -> RightHand
    {0, 1},   // Hips -> LeftUpperLeg
    {1, 3},   // LeftUpperLeg -> LeftLowerLeg
    {3, 5},   // LeftLowerLeg -> LeftFoot
    {0, 2},   // Hips -> RightUpperLeg
    {2, 4},   // RightUpperLeg -> RightLowerLeg
    {4, 6}    // RightLowerLeg -> RightFoot
};

size_t pairCount = sizeof(skeleton) / sizeof(skeleton[0]);

// enum HumanBodyBones {
//     Hips = 0,
//     LeftUpperLeg = 1,
//     RightUpperLeg = 2,
//     LeftLowerLeg = 3,
//     RightLowerLeg = 4,
//     LeftFoot = 5,
//     RightFoot = 6,
//     Spine = 7,
//     Chest = 8,
//     UpperChest = 54,
//     Neck = 9,
//     Head = 10,
//     LeftShoulder = 11,
//     RightShoulder = 12,
//     LeftUpperArm = 13,
//     RightUpperArm = 14,
//     LeftLowerArm = 15,
//     RightLowerArm = 16,
//     LeftHand = 17,
//     RightHand = 18
// };

// // ---- Bones pair for ESP ----
// struct BonePair { int from; int to; };
// BonePair simpleBonePairs[] = {
//      {Hips, Spine}, {Spine, Chest}, {Chest, UpperChest}, {UpperChest, Neck}, {Neck, Head},
//     {UpperChest, LeftShoulder}, {LeftShoulder, LeftUpperArm}, {LeftUpperArm, LeftLowerArm}, {LeftLowerArm, LeftHand},
//     {UpperChest, RightShoulder}, {RightShoulder, RightUpperArm}, {RightUpperArm, RightLowerArm}, {RightLowerArm, RightHand},
//     {Hips, LeftUpperLeg}, {LeftUpperLeg, LeftLowerLeg}, {LeftLowerLeg, LeftFoot},
//     {Hips, RightUpperLeg}, {RightUpperLeg, RightLowerLeg}, {RightLowerLeg, RightFoot}
// };