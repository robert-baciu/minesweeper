#include "MenuState.hpp"

#include <memory>
#include <TGUI/Loading/Theme.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

#include "InvalidDifficultyError.hpp"
#include "PlayingState.hpp"
#include "PlaySettings.hpp"
#include "State.hpp"

MenuState::MenuState(State::Context const &ctx) : State(ctx)
{
    auto theme = tgui::Theme::create("assets/themes/Dark.txt");
    tgui::Theme::setDefault(theme);

    ctx.getWindow().get().setSize(ctx.getWindow().getDefaultSize());

    auto const elemWidth = "80%";
    auto const elemHeight = 50.0f;

    auto title = tgui::Label::create("MINESWEEPER");
    title->setTextSize(64);
    title->setPosition("50% - (width / 2)", 40.0f);
    gui.add(title, "title");

    diffBox = tgui::ComboBox::create();
    diffBox->setSize(elemWidth, elemHeight);
    diffBox->setPosition("50% - (width / 2)", "title.bottom + 40");
    diffBox->addItem("Easy");
    diffBox->addItem("Medium");
    diffBox->addItem("Hard");
    diffBox->addItem("Extreme");
    diffBox->addItem("Custom");
    diffBox->setSelectedItemByIndex(0);
    gui.add(diffBox, "difficulty");

    auto customGroup = tgui::Group::create();
    auto customGroupSpacing = 10.0f;
    customGroup->setSize(elemWidth, elemHeight * 3 + customGroupSpacing * 2);
    customGroup->setPosition("50% - (width / 2)", "difficulty.bottom + 10");

    colsEdit = tgui::EditBox::create();
    colsEdit->setDefaultText("Columns");
    colsEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    colsEdit->setSize("100%", elemHeight);
    colsEdit->setPosition(0, 0);

    rowsEdit = tgui::EditBox::create();
    rowsEdit->setDefaultText("Rows");
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
    playButton->setPosition("50% - (width / 2)", "difficulty.bottom + 20");
    gui.add(playButton, "playButton");

    auto exitButton = tgui::Button::create("EXIT");
    exitButton->setSize(elemWidth, elemHeight);
    exitButton->setPosition("50% - (width / 2)", "playButton.bottom + 10");
    gui.add(exitButton, "exitButton");

    diffBox->onItemSelect(
        [this, customGroup, playButton](tgui::String const &item)
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

    playButton->onPress([this]() { requestedPlay = true; });

    exitButton->onPress([this]() { requestedExit = true; });
}

PlaySettings MenuState::getPlaySettings() const
{
    int cols, rows;
    float mineDensity;

    std::string const diff = diffBox->getSelectedItem().toStdString();
    if (diff == "Easy")
    {
        cols = rows = 8;
        mineDensity = 0.05f;
    }
    else if (diff == "Medium")
    {
        cols = rows = 12;
        mineDensity = 0.1f;
    }
    else if (diff == "Hard")
    {
        cols = rows = 16;
        mineDensity = 0.125f;
    }
    else if (diff == "Extreme")
    {
        cols = rows = 16;
        mineDensity = 0.15f;
    }
    else if (diff == "Custom")
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
        throw InvalidDifficultyError(diff);
    }

    return PlaySettings::Builder()
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

    if (requestedPlay)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state =
            std::make_unique<PlayingState>(ctx, getPlaySettings());
        return transition;
    }

    return std::nullopt;
}

void MenuState::print(std::ostream &os) const
{
    os << "MenuState";
}
