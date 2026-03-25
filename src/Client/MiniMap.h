#define MAX_MINI_MAP		100
#define MAIN_CURRENT_MAP 0x007D60C8


void InitMiniMap();
void MiniMapCore();
void MiniMapLoad();
bool MiniMapCheck(int map);
bool MiniMapFileCheck(int map);