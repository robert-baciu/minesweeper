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
    explicit MenuState(std::unique_ptr<StateCtx> ctx_);

    void buildGui();

    void buildPlayParams();

    [[nodiscard]] std::optional<Transition> getTransition() override;

    [[nodiscard]] DifficultyParams getPlayParams() const;

    void print(std::ostream &os) const override;

  private:
    static void validateEditBox(tgui::EditBox::Ptr &editBox,
                                unsigned int minVal, unsigned int maxVal);

    static constexpr unsigned int MIN_COLS = 5;
    static constexpr unsigned int MAX_COLS = 24;
    static constexpr unsigned int MIN_ROWS = 5;
    static constexpr unsigned int MAX_ROWS = 24;
    static constexpr unsigned int MIN_DENSITY = 1;
    static constexpr unsigned int MAX_DENSITY = 80;

    std::map<Difficulty, DifficultyParams> playParams;

    bool gotoPlayClassic;
    bool gotoLeaderboard;

    tgui::ComboBox::Ptr difficultyBox;
    tgui::EditBox::Ptr colsEdit;
    tgui::EditBox::Ptr rowsEdit;
    tgui::EditBox::Ptr densityEdit;
};
