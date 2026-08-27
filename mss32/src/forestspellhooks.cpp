#include "forestspellhooks.h"
#include "categoryids.h"
#include "forestspellcontract.h"
#include "midgardid.h"
#include "settings.h"
#include "spellutils.h"
#include "strategicspell.h"
#include <intrin.h>

namespace hooks {

ForestSpellApi::IsPositionContainsObjects originalIsPositionContainsObjects = nullptr;
ForestSpellApi::CollectTiles originalCollectTiles = nullptr;

namespace {

std::uintptr_t savedEdi = 0;
bool forestChangeTerrainActive = false;

struct ActiveFlag
{
    bool& slot;
    bool previous;

    ActiveFlag(bool& s, bool value)
        : slot(s)
        , previous(s)
    {
        slot = value;
    }

    ~ActiveFlag()
    {
        slot = previous;
    }
};

bool visitorTargetsForest(std::uintptr_t visitor)
{
    if (!visitor) {
        return false;
    }

    const auto* bytes = reinterpret_cast<const char*>(visitor);
    const game::ForestSpellContract::VisitorTarget target{
        *reinterpret_cast<const int*>(bytes + 0x14),
        *reinterpret_cast<const int*>(bytes + 0x24),
    };
    return game::ForestSpellContract::targetsForest(target);
}

bool bypassCrystalAdjacent(std::uintptr_t ret, bool settingEnabled, std::uint32_t typesTotal, bool typeIsCrystal)
{
    const auto& api = forestSpellApi();
    const game::ForestSpellContract::CallSite site{api.crystalAdjCanCastReturn,
                                                   api.crystalAdjApplyReturn};
    const bool visitorForest = ret == api.crystalAdjApplyReturn && visitorTargetsForest(savedEdi);

    return game::ForestSpellContract::decide(settingEnabled, typesTotal, typeIsCrystal, ret, site,
                                             forestChangeTerrainActive, visitorForest)
           == game::ForestSpellContract::Decision::Bypass;
}

bool __fastcall isPositionContainsObjectsHookedBody(const void* plan,
                                                    int /*dummy*/,
                                                    const void* mapPosition,
                                                    const void* objectTypes,
                                                    std::uint32_t typesTotal)
{
    if (typesTotal == 1 && objectTypes) {
        const auto type = *reinterpret_cast<const game::IdType*>(objectTypes);
        const auto ret = reinterpret_cast<std::uintptr_t>(_ReturnAddress());
        if (bypassCrystalAdjacent(ret, gameSettings().forestSpellMineAdjacent, typesTotal,
                                  type == game::IdType::Crystal)) {
            return false;
        }
    }

    if (originalIsPositionContainsObjects) {
        return originalIsPositionContainsObjects(plan, mapPosition, objectTypes, typesTotal);
    }

    return false;
}

} // namespace

__declspec(naked) bool __fastcall isPositionContainsObjectsHooked(const void* /*plan*/,
                                                                  int /*dummy*/,
                                                                  const void* /*mapPosition*/,
                                                                  const void* /*objectTypes*/,
                                                                  std::uint32_t /*typesTotal*/)
{
    __asm {
        mov savedEdi, edi
        jmp isPositionContainsObjectsHookedBody
    }
}

int __stdcall collectChangeTerrainTilesHooked(void* spellId,
                                              void* a2,
                                              void* mapPosition,
                                              void* objectMap)
{
    bool allow = false;
    if (gameSettings().forestSpellMineAdjacent && spellId) {
        auto* spell = game::SpellUtilsApi::get().findSpellById(
            static_cast<game::CMidgardID*>(spellId));
        if (spell && spell->data
            && spell->data->groundCategory.id == game::GroundId::Forest) {
            allow = true;
        }
    }

    ActiveFlag flag(forestChangeTerrainActive, allow);
    if (originalCollectTiles) {
        return originalCollectTiles(spellId, a2, mapPosition, objectMap);
    }

    return 0;
}

} // namespace hooks
