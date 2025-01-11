#pragma once
#include "battle_game/core/unit.h"

namespace battle_game::unit {
class Warthoggggg : public Unit {
  public:
    Warthoggggg(GameCore *game_core, uint32_t id, uint32_t player_id);
    void Render() override;
    void Update() override;
    [[nodiscard]] bool IsHit(glm::vec2 position) const override;

  protected:
    void _move(float move_speed);
    void _rotate(float rotate_angular_speed);
    void _fire();
    [[nodiscard]] const char *UnitName() const override;
    [[nodiscard]] const char *Author() const override;

    int32_t fire_count_down_{0};
};
} // namespace battle_game::unit
