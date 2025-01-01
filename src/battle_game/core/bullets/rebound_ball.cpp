#include "battle_game/core/bullets/rebound_ball.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
ReboundBall::ReboundBall(GameCore *core,
                         uint32_t id,
                         uint32_t unit_id,
                         uint32_t player_id,
                         glm::vec2 position,
                         float rotation,
                         float damage_scale,
                         glm::vec2 velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity),
      times_(0) {
}

void ReboundBall::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/particle3.png");
  DrawModel(0);
}

void ReboundBall::Update() {
  glm::vec2 tmp = position_;
  position_ += velocity_ * kSecondPerTick;
  bool should_die = false;
  if (game_core_->IsBlockedByObstacles(position_)) {
    Obstacle *p = game_core_->GetBlockedObstacle(position_);
    if (p && times_ < MAX_TIMES_ && !(game_core_->IsBlockedByObstacles(tmp))) {
      auto pair = p->GetSurfaceNormal(tmp, position_);
      auto normal = pair.second;
      if (normal.x == 0.0f && normal.y == 0.0f)
        should_die = true;
      else {
        times_++;
        auto proj = (velocity_.x * normal.x + velocity_.y * normal.y) /
                    (normal.x * normal.x + normal.y * normal.y) * normal;
        velocity_ -= proj * 2.0f;
        auto diff = position_ - pair.first;
        proj = (diff.x * normal.x + diff.y * normal.y) /
               (normal.x * normal.x + normal.y * normal.y) * normal;
        position_ -= proj * 2.0f;
      }
    } else
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

ReboundBall::~ReboundBall() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet
