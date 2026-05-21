#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "CellGrid.hpp"
#include "State.hpp"

class LostState : public State
{
  public:
    LostState(State::Context const &ctx, CellGrid grid,
              sf::Vector2i detonatedPos);

    void handleEvent(std::optional<sf::Event> const &event) override;

    void update(double dt) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    static constexpr sf::Color MINE_COLOR = {15, 15, 15};
    static constexpr sf::Color MINE_FLAGGED_COLOR = {0, 190, 0};
    static constexpr sf::Color MINE_DETONATED_COLOR = {190, 0, 0};
    static constexpr sf::Color FLAG_MISPLACE_COLOR = {190, 0, 0};

    CellGrid grid;
    sf::Vector2i detonatedPos;

    sf::RectangleShape overlay;
    sf::Sprite flagSprite;

    bool transitionToMenu = false;

    double totalTime = 0.0f;
};
