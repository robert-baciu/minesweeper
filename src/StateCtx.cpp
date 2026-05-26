#include "StateCtx.hpp"

StateCtx::StateCtx(AssetManager const &assets, Window &window)
    : assets(assets),
      window(window)
{
}

AssetManager const &StateCtx::getAssets() const
{
    return assets;
}

Window &StateCtx::getWindow() const
{
    return window;
}

std::ostream &operator<<(std::ostream &os, StateCtx const &ctx)
{
    os << "State::Context[assets=" << ctx.getAssets()
       << ", window=" << ctx.getWindow() << "]";
    return os;
}
