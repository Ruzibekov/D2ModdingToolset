#ifndef FORESTSPELLHOOKS_H
#define FORESTSPELLHOOKS_H

#include "forestspellapi.h"

namespace hooks {

extern ForestSpellApi::IsPositionContainsObjects originalIsPositionContainsObjects;
extern ForestSpellApi::CollectTiles originalCollectTiles;

bool __fastcall isPositionContainsObjectsHooked(const void* plan,
                                                int dummy,
                                                const void* mapPosition,
                                                const void* objectTypes,
                                                std::uint32_t typesTotal);

int __stdcall collectChangeTerrainTilesHooked(void* spellId,
                                              void* a2,
                                              void* mapPosition,
                                              void* objectMap);

} // namespace hooks

#endif
