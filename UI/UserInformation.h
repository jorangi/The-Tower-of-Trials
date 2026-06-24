#pragma once
#include "Entity/Player.h"
#include "UI/IUI.h"
#include "UI/Presenter/UserInformationPresenter.h"
#include "UI/View/UserInformationView.h"
#include <memory>

namespace TTOT::UI {
class UserInformation : public IUI {
private:
  TTOT::UI::View::UserInformationView view;
  std::unique_ptr<TTOT::UI::Presenter::UserInformationPresenter> presenter;

public:
  UserInformation(const TTOT::Entities::Player &currStates) {
    presenter = std::make_unique<TTOT::UI::Presenter::UserInformationPresenter>(
        view, const_cast<TTOT::Entities::Player &>(currStates));
  }
  ~UserInformation() override = default;

  ftxui::Element Render() override {
    presenter->UpdateAll();
    return view.Render();
  }
};
} // namespace TTOT::UI