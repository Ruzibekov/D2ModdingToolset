#ifndef FORESTSPELLCONTRACT_H
#define FORESTSPELLCONTRACT_H

#include <cstdint>

namespace game {
namespace ForestSpellContract {

enum class Decision
{
    Vanilla,
    Bypass,
};

struct CallSite
{
    std::uintptr_t canCastReturn;
    std::uintptr_t applyReturn;
};

struct VisitorTarget
{
    int groundId;
    int area;
};

static const int forestGroundId = 1;
static const int minSpellArea = 1;
static const int maxSpellArea = 32;

inline bool targetsForest(const VisitorTarget& visitor)
{
    return visitor.groundId == forestGroundId && visitor.area >= minSpellArea
           && visitor.area <= maxSpellArea;
}

inline Decision decide(bool settingEnabled,
                       std::uint32_t typesTotal,
                       bool typeIsCrystal,
                       std::uintptr_t returnAddress,
                       const CallSite& site,
                       bool collectingForestTiles,
                       bool visitorTargetsForest)
{
    if (!settingEnabled || typesTotal != 1 || !typeIsCrystal) {
        return Decision::Vanilla;
    }

    if (site.canCastReturn && returnAddress == site.canCastReturn) {
        return collectingForestTiles ? Decision::Bypass : Decision::Vanilla;
    }

    if (site.applyReturn && returnAddress == site.applyReturn) {
        return visitorTargetsForest ? Decision::Bypass : Decision::Vanilla;
    }

    return Decision::Vanilla;
}

} // namespace ForestSpellContract
} // namespace game

#endif
