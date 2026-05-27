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
    os << "StateCtx[";
    os << "assets=" << ctx.getAssets() << ", ";
    os << "window=" << ctx.getWindow();
    os << "]";
    return os;
}
