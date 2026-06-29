#include "UI/Presenter/UserInformationPresenter.h"
#include "Component/ClassComponent.h"
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"

using namespace TTOT::UI::Presenter;
using namespace TTOT::UI::View;
using namespace TTOT::Entities;

namespace TTOT::UI::Presenter {
UserInformationPresenter::UserInformationPresenter(UserInformationView &view,
                                                   Player &model)
    : view(view), model(model) {
  strListenerId = model.GetComponent<Components::StatComponent>()
                      ->GetSTRStat()
                      .GetOnValueChanged()
                      .AddListener([this](const int) { UpdateSTR(); });
  dexListenerId = model.GetComponent<Components::StatComponent>()
                      ->GetDEXStat()
                      .GetOnValueChanged()
                      .AddListener([this](const int) { UpdateDEX(); });
  intListenerId = model.GetComponent<Components::StatComponent>()
                      ->GetINTStat()
                      .GetOnValueChanged()
                      .AddListener([this](const int) { UpdateINT(); });
  wisListenerId = model.GetComponent<Components::StatComponent>()
                      ->GetWISStat()
                      .GetOnValueChanged()
                      .AddListener([this](const int) { UpdateWIS(); });
  chaListenerId = model.GetComponent<Components::StatComponent>()
                      ->GetCHAStat()
                      .GetOnValueChanged()
                      .AddListener([this](const int) { UpdateCHA(); });
  defListenerId = model.GetComponent<Components::StatComponent>()
                      ->GetDEFStat()
                      .GetOnValueChanged()
                      .AddListener([this](const int) { UpdateDEF(); });
  spdListenerId = model.GetComponent<Components::StatComponent>()
                      ->GetSPDStat()
                      .GetOnValueChanged()
                      .AddListener([this](const int) { UpdateSPD(); });
  hpListenerId = model.GetComponent<Components::HealthComponent>()
                     ->GetOnHealthChanged()
                     .AddListener([this](const int) { UpdateHP(); });
  maxHpListenerId = model.GetComponent<Components::HealthComponent>()
                        ->GetMaxHP()
                        .GetOnValueChanged()
                        .AddListener([this](const int) { UpdateHP(); });
  mpListenerId = model.GetComponent<Components::ManaComponent>()
                     ->GetOnManaChanged()
                     .AddListener([this](const int) { UpdateMP(); });
  maxMpListenerId = model.GetComponent<Components::ManaComponent>()
                        ->GetMaxMP()
                        .GetOnValueChanged()
                        .AddListener([this](const int) { UpdateMP(); });
  moneyListenerId = model.GetComponent<Components::MoneyComponent>()
                        ->GetOnMoneyChanged()
                        .AddListener([this](const int) { UpdateMoney(); });
}
UserInformationPresenter::~UserInformationPresenter() {
  model.GetComponent<Components::StatComponent>()
      ->GetSTRStat()
      .GetOnValueChanged()
      .RemoveListener(strListenerId);
  model.GetComponent<Components::StatComponent>()
      ->GetDEXStat()
      .GetOnValueChanged()
      .RemoveListener(dexListenerId);
  model.GetComponent<Components::StatComponent>()
      ->GetINTStat()
      .GetOnValueChanged()
      .RemoveListener(intListenerId);
  model.GetComponent<Components::StatComponent>()
      ->GetWISStat()
      .GetOnValueChanged()
      .RemoveListener(wisListenerId);
  model.GetComponent<Components::StatComponent>()
      ->GetCHAStat()
      .GetOnValueChanged()
      .RemoveListener(chaListenerId);
  model.GetComponent<Components::StatComponent>()
      ->GetDEFStat()
      .GetOnValueChanged()
      .RemoveListener(defListenerId);
  model.GetComponent<Components::StatComponent>()
      ->GetSPDStat()
      .GetOnValueChanged()
      .RemoveListener(spdListenerId);
  model.GetComponent<Components::HealthComponent>()
      ->GetOnHealthChanged()
      .RemoveListener(hpListenerId);
  model.GetComponent<Components::HealthComponent>()
      ->GetMaxHP()
      .GetOnValueChanged()
      .RemoveListener(maxHpListenerId);
  model.GetComponent<Components::ManaComponent>()
      ->GetOnManaChanged()
      .RemoveListener(mpListenerId);
  model.GetComponent<Components::ManaComponent>()
      ->GetMaxMP()
      .GetOnValueChanged()
      .RemoveListener(maxMpListenerId);
  model.GetComponent<Components::MoneyComponent>()
      ->GetOnMoneyChanged()
      .RemoveListener(moneyListenerId);
}
void UserInformationPresenter::UpdateName() { view.SetName(model.GetName()); }
void UserInformationPresenter::UpdateGender() {
  view.SetGender(model.GetGender());
}
void UserInformationPresenter::UpdateClass() {
  view.SetClassName(model.GetComponent<Components::ClassComponent>()
                        ->GetClassInfo()
                        ->GetName());
}
void UserInformationPresenter::UpdateMoney() {
  view.SetMoney(model.GetComponent<Components::MoneyComponent>()->GetMoney());
}
void UserInformationPresenter::UpdateHP() {
  view.SetHP(
      model.GetComponent<Components::HealthComponent>()->GetCurrentHP(),
      model.GetComponent<Components::HealthComponent>()->GetMaxHP().GetValue());
}
void UserInformationPresenter::UpdateMP() {
  view.SetMP(
      model.GetComponent<Components::ManaComponent>()->GetCurrentMP(),
      model.GetComponent<Components::ManaComponent>()->GetMaxMP().GetValue());
}
void UserInformationPresenter::UpdateSTR() {
  view.SetStr(model.GetComponent<Components::StatComponent>()->GetSTR());
}
void UserInformationPresenter::UpdateDEX() {
  view.SetDex(model.GetComponent<Components::StatComponent>()->GetDEX());
}
void UserInformationPresenter::UpdateINT() {
  view.SetInt(model.GetComponent<Components::StatComponent>()->GetINT());
}
void UserInformationPresenter::UpdateWIS() {
  view.SetWis(model.GetComponent<Components::StatComponent>()->GetWIS());
}
void UserInformationPresenter::UpdateCHA() {
  view.SetCha(model.GetComponent<Components::StatComponent>()->GetCHA());
}
void UserInformationPresenter::UpdateDEF() {
  view.SetDef(model.GetComponent<Components::StatComponent>()->GetDEF());
}
void UserInformationPresenter::UpdateSPD() {
  view.SetSpd(model.GetComponent<Components::StatComponent>()->GetSPD());
}

void UserInformationPresenter::UpdateAll() {
  UpdateName();
  UpdateGender();
  UpdateClass();
  UpdateMoney();
  UpdateHP();
  UpdateMP();
  UpdateSTR();
  UpdateDEX();
  UpdateINT();
  UpdateWIS();
  UpdateCHA();
  UpdateDEF();
  UpdateSPD();
}
} // namespace TTOT::UI::Presenter