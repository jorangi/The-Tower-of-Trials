#include "Core/SaveManager.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include "Core/Event/SaveRequestEvent.h"
#include "Core/Event/SaveCompleteEvent.h"

using namespace TTOT::Core;
using namespace std;

namespace TTOT::Core
{
    void SaveManager::OnSaveRequest(int slotIndex, MasterSaveData saveData, GameContext& context)
    {
        try
        {
            nlohmann::json json = saveData;
            string filename = "Save_Slot_" + to_string(slotIndex) + ".json";

            ofstream file(filename);
            if(!file.is_open())
            {
                cerr << "[SaveManager] 파일을 생성하거나 열 수 없습니다: " << filename << endl;
                return;
            }
            file << json.dump(4);
            file.close();
            context.eventBus.Publish(Events::SaveCompletevent{slotIndex, true, ""});
        }
        catch(const exception& e)
        {
            cerr << "[SaveManager] 세이브 중 오류 발생: " << e.what() << endl;
            context.eventBus.Publish(Events::SaveCompletevent{slotIndex, false, e.what()});
        }
    }
}