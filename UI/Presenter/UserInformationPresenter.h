#pragma once
#include "Entity/Player.h"
#include "UI/View/UserInformationView.h"

namespace TTOT::UI::Presenter {
class UserInformationPresenter {
private:
  TTOT::UI::View::UserInformationView &view;
  TTOT::Entities::Player &model;
  uint32_t hpListenerId = 0;
  uint32_t maxHpListenerId = 0;
  uint32_t mpListenerId = 0;
  uint32_t maxMpListenerId = 0;
  uint32_t moneyListenerId = 0;
  uint32_t strListenerId = 0;
  uint32_t dexListenerId = 0;
  uint32_t intListenerId = 0;
  uint32_t wisListenerId = 0;
  uint32_t chaListenerId = 0;
  uint32_t defListenerId = 0;
  uint32_t spdListenerId = 0;

public:
  UserInformationPresenter(TTOT::UI::View::UserInformationView &view,
                           TTOT::Entities::Player &model);
  ~UserInformationPresenter();
  void UpdateName();
  void UpdateGender();
  void UpdateClass();
  void UpdateMoney();
  void UpdateHP();
  void UpdateMP();
  void UpdateSTR();
  void UpdateDEX();
  void UpdateINT();
  void UpdateWIS();
  void UpdateCHA();
  void UpdateDEF();
  void UpdateSPD();
  void UpdateAll();
};
} // namespace TTOT::UI::Presenter