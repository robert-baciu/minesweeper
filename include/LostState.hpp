#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "GameState.hpp"

class LostState : public GameState
{
  public:
    explicit LostState(std::unique_ptr<StateCtx> gameCtx_);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() override;

    void print(std::ostream &os) const override;

  private:
    static constexpr sf::Color MINE_COLOR{15, 15, 15};
    static constexpr sf::Color MINE_FLAGGED_COLOR{0, 190, 0};
    static constexpr sf::Color MINE_DETONATED_COLOR{190, 0, 0};
    static constexpr sf::Color FLAG_WRONG_COLOR{190, 0, 0};

    bool gotoMenu;
};
