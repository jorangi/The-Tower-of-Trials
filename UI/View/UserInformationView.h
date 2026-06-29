#pragma once
#include "UI/IUI.h"

namespace TTOT::UI::View {
class UserInformationView : public IUI {
private:
  std::string name = "";
  std::string className = "";
  int money = 0;
  int currentHP = 0;
  int maxHP = 0;
  int currentMP = 0;
  int maxMP = 0;
  int str = 0;
  int dex = 0;
  int _int = 0;
  int wis = 0;
  int cha = 0;
  int def = 0;
  int spd = 0;
  bool gender = false;

public:
  void SetName(const std::string &name) { this->name = name; }
  void SetClassName(const std::string &className) {
    this->className = className;
  }
  void SetMoney(int money) { this->money = money; }
  void SetHP(int currentHP, int maxHP) {
    this->currentHP = currentHP;
    this->maxHP = maxHP;
  }
  void SetMP(int currentMP, int maxMP) {
    this->currentMP = currentMP;
    this->maxMP = maxMP;
  }
  void SetStr(int str) { this->str = str; }
  void SetDex(int dex) { this->dex = dex; }
  void SetInt(int _int) { this->_int = _int; }
  void SetWis(int wis) { this->wis = wis; }
  void SetCha(int cha) { this->cha = cha; }
  void SetDef(int def) { this->def = def; }
  void SetSpd(int spd) { this->spd = spd; }
  void SetGender(bool gender) { this->gender = gender; }
  ftxui::Element Render() override;
};
} // namespace TTOT::UI::View