#pragma once

#include "AssetManager.hpp"
#include "Window.hpp"

class StateCtx
{
  public:
    explicit StateCtx(AssetManager const &assets, Window &window);

    StateCtx(StateCtx const &other) = default;

    virtual ~StateCtx() = default;

    [[nodiscard]] AssetManager const &getAssets() const;

    [[nodiscard]] Window &getWindow() const;

    friend std::ostream &operator<<(std::ostream &os, StateCtx const &ctx);

  private:
    AssetManager const &assets;
    Window &window;
};

using StateCtxPtr = std::shared_ptr<StateCtx>;
