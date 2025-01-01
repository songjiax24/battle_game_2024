#pragma once
#include "cstdint"
#include "input_data.h"

namespace battle_game {
class GameCore;
class Player {
 public:
  Player(GameCore *game_core, uint32_t id);
  [[nodiscard]] uint32_t GetId() const {
    return id_;
  }
  void SetInputData(const InputData &input_data) {
    input_data_ = input_data;
  }
  [[nodiscard]] const InputData &GetInputData() const {
    return input_data_;
  }
  [[nodiscard]] const InputData &GetPreviousInputData() const {
    return previous_input_data_;
  }
  [[nodiscard]] uint32_t GetPrimaryUnitId() const {
    return primary_unit_id_;
  }
  int &SelectedUnit() {
    return selected_unit_;
  }

  void Update();
  [[nodiscard]] uint32_t GetResurrectionCountDown() const {
    return resurrection_count_down_;
  }

  void SetFirstPersonPerspective(bool first_person_perspective) {
    first_person_perspective_ = first_person_perspective;
  }
  void ChangeFirstPersonPerspective();
  [[nodiscard]] bool GetFirstPersonPerspective() const {
    return first_person_perspective_;
  }

  void SetHelper(bool helper) {
    helper_ = helper;
  }
  void ChangeHelper();
  [[nodiscard]] bool GetHelper() const {
    return helper_;
  }

 private:
  GameCore *game_core_{};
  uint32_t id_{};
  InputData input_data_{};
  InputData previous_input_data_{};
  uint32_t primary_unit_id_{};
  uint32_t resurrection_count_down_{1};
  int selected_unit_{0};
  bool first_person_perspective_{false};
  bool helper_{false};
};
}  // namespace battle_game
