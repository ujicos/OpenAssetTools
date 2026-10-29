#include "StaticModelPlacementsDumperT5.h"

#include <filesystem>
#include <nlohmann/json.hpp>

void gfx_world::StaticModelPlacementsDumperT5::DumpAsset(AssetDumpingContext& context,
                                                       const XAssetInfo<T5::GfxWorld>& asset)
{
    const auto* world = asset.Asset();
    if (world->dpvs.smodelCount && !world->dpvs.smodelDrawInsts)
    {
        con::error("Missing static-model draw instances in GfxWorld \"{}\"", asset.m_name);
        return;
    }

    nlohmann::ordered_json root;
    root["_type"] = "gfxworld_static_model_placements";
    root["_version"] = 1;
    root["name"] = asset.m_name;
    root["coordinateSystem"] = "native BO1 inches and axes";
    auto& models = root["models"] = nlohmann::ordered_json::array();
    for (unsigned i = 0; i < world->dpvs.smodelCount; ++i)
    {
        const auto& instance = world->dpvs.smodelDrawInsts[i];
        if (!instance.model || !instance.model->name)
            continue;

        const auto& p = instance.placement;
        models.push_back({{"index", i},
                          {"name", instance.model->name},
                          {"origin", {p.origin[0], p.origin[1], p.origin[2]}},
                          {"axis", {{p.axis[0][0], p.axis[0][1], p.axis[0][2]},
                                    {p.axis[1][0], p.axis[1][1], p.axis[1][2]},
                                    {p.axis[2][0], p.axis[2][1], p.axis[2][2]}}},
                          {"scale", p.scale}});
    }

    const auto filename = "gfxworld/" + std::filesystem::path(asset.m_name).filename().string() + ".static_models.json";
    const auto output = context.OpenAssetFile(filename);
    if (output)
        *output << root.dump(4) << '\n';
}
