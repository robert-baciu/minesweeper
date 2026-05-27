#include "PausedState.hpp"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>

#include "MenuState.hpp"
#include "PlayingState.hpp"

PausedState::PausedState(std::unique_ptr<StateCtx> gameCtx_)
    : GameState(std::move(gameCtx_)),
      resume(false),
      gotoMenu(false)
{
    gameCtx->getHeader().stopClock();

    auto &gui = ctx->getWindow().getGui();

    auto title = tgui::Label::create("PAUSED");
    title->setTextSize(64);
    title->setOrigin(0.5f, 0.0f);
    title->setPosition("50%", 40.0f);

    auto resumeBtn = tgui::Button::create("RESUME");
    resumeBtn->setSize("80%", 64.0f);
    resumeBtn->setOrigin(0.5f, 0.0f);
    resumeBtn->setPosition("50%", "Title.bottom + 40");

    auto menuBtn = tgui::Button::create("MENU");
    menuBtn->setSize("80%", 64.0f);
    menuBtn->setOrigin(0.5f, 0.0f);
    menuBtn->setPosition("50%", "ResumeBtn.bottom + 10");

    auto exitBtn = tgui::Button::create("EXIT");
    exitBtn->setSize("80%", 64.0f);
    exitBtn->setOrigin(0.5f, 0.0f);
    exitBtn->setPosition("50%", "MenuBtn.bottom + 10");

    resumeBtn->onPress(
        [this]()
        {
            gameCtx->getHeader().startClock();
            resume = true;
        });
    menuBtn->onPress([this]() { gotoMenu = true; });
    exitBtn->onPress([this]() { requestedExit = true; });

    gui.add(title, "Title");
    gui.add(resumeBtn, "ResumeBtn");
    gui.add(menuBtn, "MenuBtn");
    gui.add(exitBtn, "ExitBtn");
}

std::optional<State::Transition> PausedState::getTransition()
{
    if (requestedExit)
    {
        State::Transition transition;
        transition.action = State::Action::Exit;
        return transition;
    }

    if (resume)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<PlayingState>(std::move(ctx));
        return transition;
    }

    if (gotoMenu)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<MenuState>(std::move(ctx));
        return transition;
    }

    return std::nullopt;
}
