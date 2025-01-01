#include "battle_game/core/player.h"

#include "battle_game/core/game_core.h"

namespace battle_game {
Player::Player(GameCore *game_core, uint32_t id)
    : game_core_(game_core), id_(id) {
}

void Player::Update() {
  auto primary_unit = game_core_->GetUnit(primary_unit_id_);
  if (!primary_unit) {
    if (!resurrection_count_down_) {
      resurrection_count_down_ = kTickPerSecond * 5;  // Respawn after 5 seconds
    }
    resurrection_count_down_--;
    if (!resurrection_count_down_) {
      primary_unit_id_ = game_core_->AllocatePrimaryUnit(id_);
    }
  }
  ChangeFirstPersonPerspective();
  ChangeHelper();
  previous_input_data_ = input_data_;
}

void Player::ChangeFirstPersonPerspective() {
  if (input_data_.key_down[GLFW_KEY_C] &&
      !(previous_input_data_.key_down[GLFW_KEY_C])) {
    first_person_perspective_ = !first_person_perspective_;
  }
}

void Player::ChangeHelper() {
  if (input_data_.key_down[GLFW_KEY_X] &&
      !(previous_input_data_.key_down[GLFW_KEY_X])) {
    helper_ = !helper_;
  }
}
}  // namespace battle_game
