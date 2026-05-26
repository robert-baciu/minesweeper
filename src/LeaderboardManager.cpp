#include "LeaderboardManager.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include "csv.hpp" // IWYU pragma: keep
#include "LeaderboardEntry.hpp"

std::vector<LeaderboardEntry> LeaderboardManager::loadAll()
{
    std::vector<LeaderboardEntry> scores;

    if (!std::filesystem::exists(LEADERBOARD_FILE))
    {
        return scores;
    }

    csv::CSVReader reader(LEADERBOARD_FILE);
    for (csv::CSVRow &row : reader)
    {
        std::string difficulty = row["difficulty"].get<std::string>();
        auto time = row["time"].get<float>();

        LeaderboardEntry entry{difficulty, time};
        scores.push_back(entry);
    }
    return scores;
}

void LeaderboardManager::save(LeaderboardEntry const &entry)
{
    save(std::vector<LeaderboardEntry>{entry});
}

void LeaderboardManager::save(std::vector<LeaderboardEntry> const &entries)
{
    bool missingHeader = false;
    if (!std::filesystem::exists(LEADERBOARD_FILE) ||
        std::filesystem::file_size(LEADERBOARD_FILE) == 0)
    {
        missingHeader = true;
    }

    std::ofstream out(LEADERBOARD_FILE, std::ios::binary | std::ios::app);
    auto writer = csv::make_csv_writer(out).set_auto_flush(false);

    if (missingHeader)
    {
        writer << std::vector<std::string>{"difficulty", "time"};
    }

    for (auto const &entry : entries)
    {
        std::string difficulty = entry.getDifficulty();
        std::string time = std::to_string(entry.getTime());

        writer << std::vector<std::string>{difficulty, time};
    }

    writer.flush();
}
