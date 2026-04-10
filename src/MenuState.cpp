#include "MenuState.hpp"

#include <memory>
#include <TGUI/Loading/Theme.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

#include "PlayingState.hpp"
#include "State.hpp"

MenuState::MenuState(StateContext const &ctx) : State(ctx)
{
    auto theme = tgui::Theme::create("assets/themes/Dark.txt");
    tgui::Theme::setDefault(theme);

    ctx.window.get().setSize(ctx.window.getDefaultSize());

    auto const elemWidth = "80%";
    auto const elemHeight = 50.0f;

    auto title = tgui::Label::create("MINESWEEPER");
    title->setTextSize(64);
    title->setPosition("50% - (width / 2)", 40.0f);
    gui.add(title, "title");

    auto difficulty = tgui::ComboBox::create();
    difficulty->setSize(elemWidth, elemHeight);
    difficulty->setPosition("50% - (width / 2)", "title.bottom + 40");
    difficulty->addItem("Easy");
    difficulty->addItem("Medium");
    difficulty->addItem("Hard");
    difficulty->addItem("Extreme");
    difficulty->addItem("Custom");
    difficulty->setSelectedItemByIndex(0);
    gui.add(difficulty, "difficulty");

    auto customGroup = tgui::Group::create();
    auto customGroupSpacing = 10.0f;
    customGroup->setSize(elemWidth, elemHeight * 3 + customGroupSpacing * 2);
    customGroup->setPosition("50% - (width / 2)", "difficulty.bottom + 10");

    auto widthEdit = tgui::EditBox::create();
    widthEdit->setDefaultText("Width");
    widthEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    widthEdit->setSize("100%", elemHeight);
    widthEdit->setPosition(0, 0);

    auto heightEdit = tgui::EditBox::create();
    heightEdit->setDefaultText("Height");
    heightEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    heightEdit->setSize("100%", elemHeight);
    heightEdit->setPosition(0, elemHeight + customGroupSpacing);

    auto mineEdit = tgui::EditBox::create();
    mineEdit->setDefaultText("Mines");
    mineEdit->setInputValidator(tgui::EditBox::Validator::UInt);
    mineEdit->setSize("100%", elemHeight);
    mineEdit->setPosition(0, (elemHeight + customGroupSpacing) * 2);

    customGroup->add(widthEdit);
    customGroup->add(heightEdit);
    customGroup->add(mineEdit);

    auto playButton = tgui::Button::create("PLAY");
    playButton->setSize(elemWidth, elemHeight);
    playButton->setPosition("50% - (width / 2)", "difficulty.bottom + 20");
    gui.add(playButton, "playButton");

    auto exitButton = tgui::Button::create("EXIT");
    exitButton->setSize(elemWidth, elemHeight);
    exitButton->setPosition("50% - (width / 2)", "playButton.bottom + 10");
    gui.add(exitButton, "exitButton");

    difficulty->onItemSelect(
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

std::optional<State::Transition> MenuState::getTransition() const
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
        // TODO: PlaySettings: cols, rows, mineCount, startSafeDistance
        transition.state = std::make_unique<PlayingState>(ctx, 16, 8);
        return transition;
    }

    return std::nullopt;
}

void MenuState::print(std::ostream &os) const
{
    os << "MenuState";
}
