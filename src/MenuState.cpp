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

    buildGui();
    buildPlayParams();
}

void MenuState::buildGui()
{
    auto ELEM_WIDTH = "80%";
    auto ELEM_HEIGHT = 50.0f;

    auto &gui = ctx->getWindow().getGui();

    // TODO: setOrigin

    auto title = tgui::Label::create("MINESWEEPER");
    title->setTextSize(64);
    title->setPosition("50% - (width / 2)", 40.0f);

    difficultyBox = tgui::ComboBox::create();
    difficultyBox->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    difficultyBox->setPosition("50% - (width / 2)", "TitleLabel.bottom + 40");
    for (std::string const &difficultyStr : DifficultyUtil::allStrings())
    {
        difficultyBox->addItem(difficultyStr);
    }
    difficultyBox->setSelectedItemByIndex(1);

    auto GROUP_PADDING_V = 10.0f;
    auto group = tgui::Group::create();
    group->setSize(ELEM_WIDTH, ELEM_HEIGHT * 3 + GROUP_PADDING_V * 2);
    group->setPosition("50% - (width / 2)", "DifficultyBox.bottom + 10");

    colsEdit = tgui::EditBox::create();
    colsEdit->setDefaultText("Columns [>= 8]");
    colsEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    colsEdit->setSize("100%", ELEM_HEIGHT);
    colsEdit->setPosition(0, 0);

    rowsEdit = tgui::EditBox::create();
    rowsEdit->setDefaultText("Rows [>= 8]");
    rowsEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    rowsEdit->setSize("100%", ELEM_HEIGHT);
    rowsEdit->setPosition(0, ELEM_HEIGHT + GROUP_PADDING_V);

    densityEdit = tgui::EditBox::create();
    densityEdit->setDefaultText("Mine density [0-100]");
    densityEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    densityEdit->setSize("100%", ELEM_HEIGHT);
    densityEdit->setPosition(0, (ELEM_HEIGHT + GROUP_PADDING_V) * 2);

    auto playClassicBtn = tgui::Button::create("CLASSIC");
    playClassicBtn->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    playClassicBtn->setPosition("50% - (width / 2)",
                                "DifficultyBox.bottom + 10");

    auto leaderboardBtn = tgui::Button::create("LEADERBOARD");
    leaderboardBtn->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    leaderboardBtn->setPosition("50% - (width / 2)",
                                "PlayClassicButton.bottom + 10");

    auto exitBtn = tgui::Button::create("EXIT");
    exitBtn->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    exitBtn->setPosition("50% - (width / 2)", "LeaderboardButton.bottom + 10");

    group->add(colsEdit);
    group->add(rowsEdit);
    group->add(densityEdit);

    gui.add(title, "TitleLabel");
    gui.add(difficultyBox, "DifficultyBox");
    gui.add(playClassicBtn, "PlayClassicButton");
    gui.add(leaderboardBtn, "LeaderboardButton");
    gui.add(exitBtn, "ExitButton");

    difficultyBox->onItemSelect(
        // TODO:
        [&gui, group, playClassicBtn](tgui::String const &item)
        {
            if (item == "Custom")
            {
                if (group->getParent() == nullptr)
                {
                    gui.add(group, "CustomParamsGroup");
                    playClassicBtn->setPosition(
                        "50% - (width / 2)", "CustomParamsGroup.bottom + 10");
                }
            }
            else
            {
                if (group->getParent() != nullptr)
                {
                    gui.remove(group);
                    playClassicBtn->setPosition(
                        "50% - (width / 2)", "CustomParamsGroup.bottom + 10");
                }
            }
        });

    playClassicBtn->onPress([this]() { gotoPlayClassic = true; });

    leaderboardBtn->onPress([this]() { gotoLeaderboard = true; });

    exitBtn->onPress([this]() { requestedExit = true; });
}

void MenuState::buildPlayParams()
{
    playParams.insert({Difficulty::Easy, DifficultyParams::Builder()
                                             .withCols(8)
                                             .withRows(8)
                                             .withMineDensity(0.1f)
                                             .build()});

    playParams.insert({Difficulty::Medium, DifficultyParams::Builder()
                                               .withCols(12)
                                               .withRows(12)
                                               .withMineDensity(0.1f)
                                               .build()});

    playParams.insert({Difficulty::Hard, DifficultyParams::Builder()
                                             .withCols(16)
                                             .withRows(16)
                                             .withMineDensity(0.125f)
                                             .build()});

    playParams.insert({Difficulty::Extreme, DifficultyParams::Builder()
                                                .withCols(20)
                                                .withRows(20)
                                                .withMineDensity(0.15f)
                                                .build()});
}

DifficultyParams MenuState::getPlayParams() const
{
    Difficulty difficulty = DifficultyUtil::fromString(
        difficultyBox->getSelectedItem().toStdString());

    if (difficulty != Difficulty::Custom)
    {
        return playParams.at(difficulty);
    }

    // TODO: Validation
    int cols = std::stoi(colsEdit->getText().toStdString());
    int rows = std::stoi(rowsEdit->getText().toStdString());
    float mineDensity =
        static_cast<float>(std::stoul(densityEdit->getText().toStdString())) /
        100.0f;

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

    if (gotoLeaderboard)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<LeaderboardState>(std::move(ctx));
        return transition;
    }

    if (gotoPlayClassic)
    {
        Difficulty difficulty = DifficultyUtil::fromString(
            difficultyBox->getSelectedItem().toStdString());

        auto gameCtx =
            std::make_shared<GameStateCtx>(ctx, difficulty, getPlayParams());

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
