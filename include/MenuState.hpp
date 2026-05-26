#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/EditBox.hpp>

#include "Difficulty.hpp"
#include "DifficultyParams.hpp"
#include "State.hpp"

class MenuState : public State
{
  public:
    explicit MenuState(StateCtxPtr ctx_);

    void buildGui();
    void buildPlayParams();

    [[nodiscard]] DifficultyParams getPlayParams() const;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    std::map<Difficulty, DifficultyParams> playParams;

    bool gotoPlayClassic = false;
    bool gotoLeaderboard = false;

    tgui::ComboBox::Ptr difficultyBox;
    tgui::EditBox::Ptr colsEdit;
    tgui::EditBox::Ptr rowsEdit;
    tgui::EditBox::Ptr densityEdit;
};
