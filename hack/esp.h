bool line = false;
void DrawEsp()
{
    ImDrawList *ImDrawList = ImGui::GetBackgroundDrawList();
    float width = (float)Width();
    float height = (float)Height();

    for (int i = 0; i < playerlist.count; i++)
    {
        void *Player = playerlist.addr[i];
        if (Player == NULL)
        {
            continue;
        }

        Vector3 playerPos = GetPosition(Player);
        Vector3 WorldPos = World(Main(), playerPos);

        if (WorldPos.z < 1.f)
        {
            continue;
        }
        ImVec2 start_point = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y);

        ImU32 red_color = IM_COL32(255, 0, 0, 255);
        float thickness = 2.0f;
        Vector2 Enemy = {WorldPos.x, WorldPos.y - height}
        if (esp)
        {
            ImDrawList->AddLine(start_point, Enemy, red_color, thickness);
        }
    }
}
