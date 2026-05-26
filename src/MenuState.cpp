#include "MenuState.hpp"

#include <memory>
#include <TGUI/Loading/Theme.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <utility>

#include "LeaderboardState.hpp"
#include "PlayingState.hpp"
#include "State.hpp"

MenuState::MenuState(StateCtxPtr ctx_)
    : State(std::move(ctx_))
{
    ctx->getWindow().get().setSize(ctx->getWindow().getDefaultSize());

    auto const elemWidth = "80%";
    auto const elemHeight = 50.0f;

    auto &gui = ctx->getWindow().getGui();

    auto title = tgui::Label::create("MINESWEEPER");
    title->setTextSize(64);
    title->setPosition("50% - (width / 2)", 40.0f);
    gui.add(title, "title");

    diffBox = tgui::ComboBox::create();
    diffBox->setSize(elemWidth, elemHeight);
    diffBox->setPosition("50% - (width / 2)", "title.bottom + 40");
    for (std::string const &difficultyStr : DifficultyUtil::allStrings())
    {
        diffBox->addItem(difficultyStr);
    }
    diffBox->setSelectedItemByIndex(1);
    gui.add(diffBox, "difficulty");

    auto customGroup = tgui::Group::create();
    auto customGroupSpacing = 10.0f;
    customGroup->setSize(elemWidth, elemHeight * 3 + customGroupSpacing * 2);
    customGroup->setPosition("50% - (width / 2)", "difficulty.bottom + 10");

    colsEdit = tgui::EditBox::create();
    colsEdit->setDefaultText("Columns [>= 8]");
    colsEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    colsEdit->setSize("100%", elemHeight);
    colsEdit->setPosition(0, 0);

    rowsEdit = tgui::EditBox::create();
    rowsEdit->setDefaultText("Rows [>= 8]");
    rowsEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    rowsEdit->setSize("100%", elemHeight);
    rowsEdit->setPosition(0, elemHeight + customGroupSpacing);

    densityEdit = tgui::EditBox::create();
    densityEdit->setDefaultText("Mine density [0-100]");
    densityEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    densityEdit->setSize("100%", elemHeight);
    densityEdit->setPosition(0, (elemHeight + customGroupSpacing) * 2);

    customGroup->add(colsEdit);
    customGroup->add(rowsEdit);
    customGroup->add(densityEdit);

    auto playButton = tgui::Button::create("PLAY");
    playButton->setSize(elemWidth, elemHeight);
    playButton->setPosition("50% - (width / 2)", "difficulty.bottom + 10");
    gui.add(playButton, "playButton");

    auto leaderboardButton = tgui::Button::create("LEADERBOARD");
    leaderboardButton->setSize(elemWidth, elemHeight);
    leaderboardButton->setPosition("50% - (width / 2)",
                                   "playButton.bottom + 10");
    gui.add(leaderboardButton, "leaderboardButton");

    auto exitButton = tgui::Button::create("EXIT");
    exitButton->setSize(elemWidth, elemHeight);
    exitButton->setPosition("50% - (width / 2)",
                            "leaderboardButton.bottom + 10");
    gui.add(exitButton, "exitButton");

    diffBox->onItemSelect(
        // TODO:
        [&gui, customGroup, playButton](tgui::String const &item)
        {
            if (item == "Custom")
            {
                if (customGroup->getParent() == nullptr)
                {
                    gui.add(customGroup, "customOpts");
                    playButton->setPosition("50% - (width / 2)",
                                            "customOpts.bottom + 20");
                }
            }
            else
            {
                if (customGroup->getParent() != nullptr)
                {
                    gui.remove(customGroup);
                    playButton->setPosition("50% - (width / 2)",
                                            "difficulty.bottom + 20");
                }
            }
        });

    playButton->onPress([this]() { transitionToPlay = true; });

    leaderboardButton->onPress([this]() { transitionToLeaderboard = true; });

    exitButton->onPress([this]() { requestedExit = true; });
}

DifficultyParams MenuState::getPlaySettings(Difficulty difficulty) const
{
    int cols, rows;
    float mineDensity;

    // TODO: Move to a map
    if (difficulty == Difficulty::Easy)
    {
        cols = rows = 8;
        mineDensity = 0.1f;
    }
    else if (difficulty == Difficulty::Medium)
    {
        cols = rows = 12;
        mineDensity = 0.1f;
    }
    else if (difficulty == Difficulty::Hard)
    {
        cols = rows = 16;
        mineDensity = 0.125f;
    }
    else if (difficulty == Difficulty::Extreme)
    {
        cols = rows = 16;
        mineDensity = 0.15f;
    }
    else if (difficulty == Difficulty::Custom)
    {
        // TODO: Validation
        cols = std::stoi(colsEdit->getText().toStdString());
        rows = std::stoi(rowsEdit->getText().toStdString());
        mineDensity = static_cast<float>(
                          std::stoul(densityEdit->getText().toStdString())) /
                      100.0f;
    }
    else
    {
        std::unreachable();
    }

    return DifficultyParams::Builder()
        .withCols(cols)
        .withRows(rows)
        .withMineDensity(mineDensity)
        .build();
}

std::optional<State::Transition> MenuState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (transitionToLeaderboard)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<LeaderboardState>(std::move(ctx));
        return transition;
    }

    if (transitionToPlay)
    {
        Difficulty selected = DifficultyUtil::fromString(
            diffBox->getSelectedItem().toStdString());
        DifficultyParams params = getPlaySettings(selected);

        auto gameCtx = std::make_shared<GameStateCtx>(ctx, params);

        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<PlayingState>(std::move(gameCtx));
        return transition;
    }

    return std::nullopt;
}

void MenuState::print(std::ostream &os) const
{
    os << "MenuState";
}
