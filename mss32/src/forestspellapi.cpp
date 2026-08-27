#include "forestspellapi.h"
#include "version.h"
#include <array>

namespace hooks {

namespace {

using Api = ForestSpellApi;

static std::array<Api, 4> functions = {{
    Api{
        (Api::IsPositionContainsObjects)0x5f69ae,
        (Api::CollectTiles)0x5dc596,
        0x4360a2,
        0x5dc9a3,
    },
    Api{
        (Api::IsPositionContainsObjects)0x5f69ae,
        (Api::CollectTiles)0x5dc596,
        0x4360a2,
        0x5dc9a3,
    },
    Api{
        (Api::IsPositionContainsObjects)0x5f5631,
        (Api::CollectTiles)0x5db2cb,
        0x435abf,
        0x5db6d8,
    },
    Api{
        (Api::IsPositionContainsObjects)nullptr,
        (Api::CollectTiles)nullptr,
        0,
        0,
    },
}};

} // namespace

const ForestSpellApi& forestSpellApi()
{
    return functions[static_cast<int>(gameVersion())];
}

} // namespace hooks
