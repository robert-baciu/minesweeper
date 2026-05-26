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

    // listView->setColumnAlignment(0, tgui::HorizontalAlignment::Center);
    // listView->setColumnAlignment(1, tgui::HorizontalAlignment::Center);

    listView->getRenderer()->setPadding(tgui::Padding(10, 10, 10, 10));

    std::vector<LeaderboardEntry> entries = LeaderboardManager::loadAll();

    std::sort(entries.begin(), entries.end(), [](auto const &x, auto const &y)
              { return x.getTime() < y.getTime(); });

    listView->setTextSize(32);
    listView->setItemHeight(32);

    std::map<std::string, std::vector<LeaderboardEntry>> groups;
    for (auto const &entry : entries)
    {
        groups[entry.getDifficulty()].push_back(entry);
    }

    for (auto &[difficulty, entries] : groups)
    {
        std::sort(entries.begin(), entries.end(),
                  [](auto const &a, auto const &b)
                  { return a.getTime() < b.getTime(); });
    }

    // TODO: all difficulties in order, maybe - for no played difficulties
    for (auto const &[difficulty, entries] : groups)
    {
        for (size_t i = 0; i < 3; i++)
        {
            auto const &entry = entries[i];

            std::string difficulty = entry.getDifficulty();

            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << entry.getTime();
            std::string time = stream.str();

            listView->addItem({difficulty, time});
        }
    }
}

void LeaderboardState::print(std::ostream &os) const
{
    os << "LeaderboardState";
}
