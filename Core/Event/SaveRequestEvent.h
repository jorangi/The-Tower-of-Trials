namespace TTOT::Core::Events
{
    struct SaveRequestEvent
    {
        int slotIndex;
        int currentFloor = 1;
        unsigned int mapSeed = 0;
        int playerX = -1;
        int playerY = -1;
    };
}