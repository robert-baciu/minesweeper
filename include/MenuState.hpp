#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/EditBox.hpp>

#include "DifficultySettings.hpp"
#include "State.hpp"

class MenuState : public State
{
  public:
    explicit MenuState(State::Context const &ctx);

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    DifficultySettings getPlaySettings() const;

    bool requestedPlay = false;

    tgui::ComboBox::Ptr diffBox;
    tgui::EditBox::Ptr colsEdit;
    tgui::EditBox::Ptr rowsEdit;
    tgui::EditBox::Ptr densityEdit;
};
