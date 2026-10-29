#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5/T5.h"

namespace gfx_world
{
    // Exports placement metadata only; does not claim to export world geometry.
    class StaticModelPlacementsDumperT5 final : public AbstractAssetDumper<T5::AssetGfxWorld>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T5::GfxWorld>& asset) override;
    };
}
