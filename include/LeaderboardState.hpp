#pragma once

#include <TGUI/Widgets/ListView.hpp>

#include "State.hpp"

class LeaderboardState : public State
{
  public:
    explicit LeaderboardState(std::unique_ptr<StateCtx> ctx);

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    void buildGui();

    void buildEntries();

    tgui::ListView::Ptr listView;

    bool gotoMenu;
};
