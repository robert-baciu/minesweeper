#include "State.hpp"

class LostState : public State
{
  public:
    explicit LostState(StateContext const &ctx);

    virtual void handleEvent(std::optional<sf::Event> const &event) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    [[nodiscard]] std::optional<Transition> getTransition() const override;

    void print(std::ostream &os) const override;

  private:
    bool backToMainMenu = false;
};
