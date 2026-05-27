#include "LeaderboardState.hpp"

#include <memory>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ListView.hpp>

#include "LeaderboardEntry.hpp"
#include "LeaderboardManager.hpp"
#include "MenuState.hpp"
#include "State.hpp"

LeaderboardState::LeaderboardState(std::unique_ptr<StateCtx> ctx)
    : State(std::move(ctx)),
      gotoMenu(false)
{
    buildGui();
    buildEntries();
}

void LeaderboardState::buildGui()
{
    auto &gui = ctx->getWindow().getGui();

    listView = tgui::ListView::create();
    listView->setSize("80%", "80% - 74");
    listView->setOrigin(0.5f, 0.0f);
    listView->setPosition("50%", "10%");

    listView->addColumn("Difficulty", 0);
    listView->addColumn("Time", 0);
    listView->setColumnExpanded(0, true);
    listView->setColumnExpanded(1, true);
    listView->setColumnAlignment(0, tgui::HorizontalAlignment::Center);
    listView->setColumnAlignment(1, tgui::HorizontalAlignment::Center);

    listView->setTextSize(32);
    listView->setItemHeight(48);

    auto menuBtn = tgui::Button::create("MENU");
    menuBtn->setSize("80%", 64.0f);
    menuBtn->setOrigin(0.5f, 0.0f);
    menuBtn->setPosition("50%", "LeaderboardList.bottom + 10");

    menuBtn->onPress([this]() { gotoMenu = true; });

    gui.add(listView, "LeaderboardList");
    gui.add(menuBtn, "MenuBtn");
}

void LeaderboardState::buildEntries()
{
    std::vector<LeaderboardEntry> allEntries = LeaderboardManager::loadAll();
    std::map<Difficulty, std::vector<LeaderboardEntry>> entriesByDifficulty;
    for (auto const &entry : allEntries)
    {
        entriesByDifficulty[entry.getDifficulty()].push_back(entry);
    }

    for (auto &[difficulty, entries] : entriesByDifficulty)
    {
        std::sort(entries.begin(), entries.end(),
                  [](auto const &a, auto const &b)
                  { return a.getTime() < b.getTime(); });
    }

    for (auto const difficulty : DifficultyUtil::all())
    {
        if (entriesByDifficulty.find(difficulty) == entriesByDifficulty.end() ||
            entriesByDifficulty[difficulty].empty())
        {
            std::string difficultyStr = DifficultyUtil::toString(difficulty);
            listView->addItem({difficultyStr, "-"});
        }
        else
        {
            auto const &entries = entriesByDifficulty.at(difficulty);

            size_t count = std::min(entries.size(), static_cast<size_t>(3));

            for (size_t i = 0; i < count; i++)
            {
                std::string difficultyStr =
                    DifficultyUtil::toString(difficulty);

                auto const &entry = entries[i];
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << entry.getTime();
                std::string timeStr = stream.str();

                listView->addItem({difficultyStr, timeStr});
            }
        }
    }
}

std::optional<State::Transition> LeaderboardState::getTransition()
{
    if (gotoMenu)
    {
        State::Transition transition;
        transition.action = State::Action::Change;
        transition.state = std::make_unique<MenuState>(std::move(ctx));
        return transition;
    }

    return std::nullopt;
}

void LeaderboardState::print(std::ostream &os) const
{
    os << "LeaderboardState";
}
