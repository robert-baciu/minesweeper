#include "MenuState.hpp"

#include <memory>
#include <TGUI/Loading/Theme.hpp>
#include <TGUI/String.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <utility>

#include "Difficulty.hpp"
#include "LeaderboardState.hpp"
#include "PlayingState.hpp"
#include "State.hpp"

MenuState::MenuState(std::unique_ptr<StateCtx> ctx_)
    : State(std::move(ctx_)),
      gotoPlayClassic(false),
      gotoLeaderboard(false)
{
    ctx->getWindow().get().setSize(ctx->getWindow().getDefaultSize());

    buildGui();
    buildPlayParams();
}

void MenuState::buildGui()
{
    auto ELEM_WIDTH = "80%";
    auto ELEM_HEIGHT = 64.0f;

    auto &gui = ctx->getWindow().getGui();

    auto title = tgui::Label::create("MINESWEEPER");
    title->setTextSize(64);
    title->setOrigin(0.5f, 0.0f);
    title->setPosition("50%", 40.0f);

    difficultyBox = tgui::ComboBox::create();
    difficultyBox->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    difficultyBox->setOrigin(0.5f, 0.0f);
    difficultyBox->setPosition("50%", "TitleLabel.bottom + 40");

    auto group = tgui::Group::create();
    group->setSize(ELEM_WIDTH, "DensityEdit.bottom");
    group->setOrigin(0.5f, 0.0f);
    group->setPosition("50%", "DifficultyBox.bottom + 10");

    colsEdit = tgui::EditBox::create();
    colsEdit->setDefaultText("Columns [" + tgui::String(MIN_COLS) + "-" +
                             tgui::String(MAX_COLS) + "]");
    colsEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    colsEdit->setSize("100%", ELEM_HEIGHT);
    colsEdit->setPosition(0, 0);

    rowsEdit = tgui::EditBox::create();
    rowsEdit->setDefaultText("Rows [" + tgui::String(MIN_ROWS) + "-" +
                             tgui::String(MAX_ROWS) + "]");
    rowsEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    rowsEdit->setSize("100%", ELEM_HEIGHT);
    rowsEdit->setPosition(0, "ColsEdit.bottom + 10");

    densityEdit = tgui::EditBox::create();
    densityEdit->setDefaultText("Mine density [" + tgui::String(MIN_DENSITY) +
                                "-" + tgui::String(MAX_DENSITY) + "]");
    densityEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    densityEdit->setSize("100%", ELEM_HEIGHT);
    densityEdit->setPosition(0, "RowsEdit.bottom + 10");

    auto playClassicBtn = tgui::Button::create("CLASSIC");
    playClassicBtn->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    playClassicBtn->setOrigin(0.5f, 0.0f);
    playClassicBtn->setPosition("50%", "DifficultyBox.bottom + 10");

    auto leaderboardBtn = tgui::Button::create("LEADERBOARD");
    leaderboardBtn->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    leaderboardBtn->setOrigin(0.5f, 0.0f);
    leaderboardBtn->setPosition("50%", "PlayClassicButton.bottom + 10");

    auto exitBtn = tgui::Button::create("EXIT");
    exitBtn->setSize(ELEM_WIDTH, ELEM_HEIGHT);
    exitBtn->setOrigin(0.5f, 0.0f);
    exitBtn->setPosition("50%", "LeaderboardButton.bottom + 10");

    group->add(colsEdit, "ColsEdit");
    group->add(rowsEdit, "RowsEdit");
    group->add(densityEdit, "DensityEdit");

    gui.add(title, "TitleLabel");
    gui.add(difficultyBox, "DifficultyBox");
    gui.add(playClassicBtn, "PlayClassicButton");
    gui.add(leaderboardBtn, "LeaderboardButton");
    gui.add(exitBtn, "ExitButton");

    for (std::string const &difficultyStr : DifficultyUtil::allStrings())
    {
        difficultyBox->addItem(difficultyStr);
    }

    difficultyBox->setSelectedItem(
        DifficultyUtil::toString(Difficulty::Medium));

    validateEditBox(colsEdit, MIN_COLS, MAX_COLS);
    validateEditBox(rowsEdit, MIN_ROWS, MAX_ROWS);
    validateEditBox(densityEdit, MIN_DENSITY, MAX_DENSITY);

    auto checkCustomParams = [this, playClassicBtn]()
    {
        auto c = colsEdit->getText();
        auto r = rowsEdit->getText();
        auto d = densityEdit->getText();

        bool isValid = !c.empty() && !r.empty() && !d.empty() &&
                       c.toUInt() >= MIN_COLS && c.toUInt() <= MAX_COLS &&
                       r.toUInt() >= MIN_ROWS && r.toUInt() <= MAX_ROWS &&
                       d.toUInt() >= MIN_DENSITY && d.toUInt() <= MAX_DENSITY;

        playClassicBtn->setEnabled(isValid);
        playClassicBtn->getRenderer()->setOpacity(isValid ? 1.0f : 0.9f);
    };

    colsEdit->onTextChange(checkCustomParams);
    rowsEdit->onTextChange(checkCustomParams);
    densityEdit->onTextChange(checkCustomParams);

    difficultyBox->onItemSelect(
        [&gui, checkCustomParams, group,
         playClassicBtn](tgui::String const &item)
        {
            if (item == "Custom")
            {
                if (group->getParent() == nullptr)
                {
                    gui.add(group, "CustomParamsGroup");
                    playClassicBtn->setPosition(
                        "50%", "CustomParamsGroup.bottom + 10");
                    checkCustomParams();
                }
            }
            else
            {
                if (group->getParent() != nullptr)
                {
                    gui.remove(group);
                    playClassicBtn->setPosition("50%",
                                                "DifficultyBox.bottom + 10");
                    playClassicBtn->setEnabled(true);
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
            std::make_unique<GameStateCtx>(*ctx, difficulty, getPlayParams());

        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<PlayingState>(std::move(gameCtx));
        return transition;
    }

    return std::nullopt;
}

void MenuState::validateEditBox(tgui::EditBox::Ptr &editBox,
                                unsigned int minVal, unsigned int maxVal)
{
    editBox->onUnfocus(
        [editBox, minVal, maxVal]()
        {
            tgui::String text = editBox->getText();
            if (text.empty())
            {
                return;
            }

            unsigned int val = text.toUInt();
            if (val < minVal)
            {
                editBox->setText(tgui::String(minVal));
            }
            else if (val > maxVal)
            {
                editBox->setText(tgui::String(maxVal));
            }
        });
}

void MenuState::print(std::ostream &os) const
{
    os << "MenuState";
}
