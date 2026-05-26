#include "LeaderboardState.hpp"

#include <TGUI/Widgets/ListView.hpp>

#include "LeaderboardEntry.hpp"
#include "LeaderboardManager.hpp"

LeaderboardState::LeaderboardState(StateCtxPtr ctx_)
    : State(std::move(ctx_))
{
    auto listView = tgui::ListView::create();
    listView->setSize("80%", "80%");
    listView->setPosition("10%", "10%");
    // TODO: listView->setOrigin();

    ctx->getWindow().getGui().add(listView, "LeaderboardList");

    float listWidth = listView->getSize().x;

    listView->addColumn("Difficulty", 0.6f * listWidth);
    listView->addColumn("Time", 0.3f * listWidth);

    listView->setColumnExpanded(0, true);
    listView->setColumnExpanded(1, true);

    listView->setColumnAlignment(0, tgui::HorizontalAlignment::Center);
    listView->setColumnAlignment(1, tgui::HorizontalAlignment::Center);

    listView->getRenderer()->setPadding(tgui::Padding(10, 10, 10, 10));

    listView->setTextSize(32);
    listView->setItemHeight(32);

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

void LeaderboardState::print(std::ostream &os) const
{
    os << "LeaderboardState";
}
