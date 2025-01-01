#pragma once
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class ReboundBall : public Bullet {
 public:
  ReboundBall(GameCore *core,
             uint32_t id,
             uint32_t unit_id,
             uint32_t player_id,
             glm::vec2 position,
             float rotation,
             float damage_scale,
             glm::vec2 velocity);
  ~ReboundBall() override;
  void Render() override;
  void Update() override;

 private:
  glm::vec2 velocity_{};
  size_t times_;
  const size_t MAX_TIMES_ = 5;
};
}  // namespace battle_game::bullet
