#include "battle_game/core/bullets/cannon_ball.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
CannonBall::CannonBall(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale,
                       glm::vec2 velocity,
                       uint32_t cannonball_health)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity), cannonball_health_(cannonball_health) {
}

void CannonBall::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/particle3.png");
  DrawModel(0);
}

void CannonBall::Update() {
  auto p = position_ + velocity_ * kSecondPerTick;
  if (game_core_->IsOutOfRange(p)) {
    if(cannonball_health_ == 0) {
      game_core_->PushEventRemoveBullet(id_);
      return;
    } else {
      cannonball_health_--;
      if (p.x < game_core_ -> GetBoundaryLow().x) {
        p.x = game_core_ -> GetBoundaryLow().x;
        velocity_.x = -velocity_.x;
      }
      if (p.x > game_core_ -> GetBoundaryHigh().x) {
        p.x = game_core_ -> GetBoundaryHigh().x;
        velocity_.x = -velocity_.x;
      }
      if (p.y < game_core_ -> GetBoundaryLow().y) {
        p.y = game_core_ -> GetBoundaryLow().y;
        velocity_.y = -velocity_.y;
      }
      if (p.y > game_core_ -> GetBoundaryHigh().y) {
        p.y = game_core_ -> GetBoundaryHigh().y;
        velocity_.y = -velocity_.y;
      }
    }
  }

  position_ = p;
  bool should_die = false;
  if (game_core_->IsBlockedByObstacles(position_)) {
    should_die = true;
  }

  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) {
      continue;
    }
    if (unit.second->IsHit(position_)) {
      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 10.0f);
      should_die = true;
    }
  }

  if (should_die) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

CannonBall::~CannonBall() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet
