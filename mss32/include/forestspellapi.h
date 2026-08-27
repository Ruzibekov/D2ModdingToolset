#ifndef FORESTSPELLAPI_H
#define FORESTSPELLAPI_H

#include <cstdint>

namespace hooks {

struct ForestSpellApi
{
    using IsPositionContainsObjects = bool(__thiscall*)(const void* plan,
                                                        const void* mapPosition,
                                                        const void* objectTypes,
                                                        std::uint32_t typesTotal);
    using CollectTiles = int(__stdcall*)(void* spellId,
                                         void* a2,
                                         void* mapPosition,
                                         void* objectMap);

    IsPositionContainsObjects isPositionContainsObjects;
    CollectTiles collectTiles;
    std::uintptr_t crystalAdjApplyReturn;
    std::uintptr_t crystalAdjCanCastReturn;
};

const ForestSpellApi& forestSpellApi();

} // namespace hooks

#endif
