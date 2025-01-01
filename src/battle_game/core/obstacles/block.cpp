#include "battle_game/core/obstacles/block.h"

namespace battle_game::obstacle {

Block::Block(GameCore *game_core,
             uint32_t id,
             glm::vec2 position,
             float rotation,
             glm::vec2 scale)
    : Obstacle(game_core, id, position, rotation) {
}

bool Block::IsBlocked(glm::vec2 p) const {
  p = WorldToLocal(p);
  return p.x <= scale_.x && p.x >= -scale_.x && p.y <= scale_.y &&
         p.y >= -scale_.y;
}

void Block::Render() {
  battle_game::SetColor(glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
  battle_game::SetTexture(0);
  battle_game::SetTransformation(position_, rotation_, scale_);
  battle_game::DrawModel(0);
}

static bool GetHorizontalIntersection(
    glm::vec2 origin,
    glm::vec2 terminus,
    float y,
    float x_begin,
    float x_end,
    std::pair<glm::vec2, glm::vec2> &intersection) {
  if ((origin.y <= y && terminus.y >= y) ||
      (origin.y >= y && terminus.y <= y)) {
    if (origin.y != terminus.y) {
      float x = (origin.x * (terminus.y - y) + terminus.x * (y - origin.y)) /
                (terminus.y - origin.y);
      if (x >= x_begin && x <= x_end) {
        intersection.first = glm::vec2{x, y};
        return true;
      }
    } else if (origin.x < x_begin && terminus.x >= x_begin) {
      intersection.first = glm::vec2{x_begin, y};
      return true;
    } else if (origin.x > x_end && terminus.x <= x_end) {
      intersection.first = glm::vec2{x_end, y};
      return true;
    } else if (origin.x >= x_begin && origin.x <= x_end) {
      intersection.first = origin;
      return true;
    }
  }
  return false;
}

static bool GetVerticalIntersection(
    glm::vec2 origin,
    glm::vec2 terminus,
    float x,
    float y_begin,
    float y_end,
    std::pair<glm::vec2, glm::vec2> &intersection) {
  if ((origin.x <= x && terminus.x >= x) ||
      (origin.x >= x && terminus.x <= x)) {
    if (origin.x != terminus.x) {
      float y = (origin.y * (terminus.x - x) + terminus.y * (x - origin.x)) /
                (terminus.x - origin.x);
      if (y >= y_begin && y <= y_end) {
        intersection.first = glm::vec2{x, y};
        return true;
      }
    } else if (origin.y < y_begin && terminus.y >= y_begin) {
      intersection.first = glm::vec2{x, y_begin};
      return true;
    } else if (origin.y > y_end && terminus.y <= y_end) {
      intersection.first = glm::vec2{x, y_end};
      return true;
    } else if (origin.y >= y_begin && origin.y <= y_end) {
      intersection.first = origin;
      return true;
    }
  }
  return false;
}

/*
 * Return the first intersection and the unit outer normal vector. There should
 * be at least one intersection, otherwise it will return
 * `std::make_pair(glm::vec2{0.0f}, glm::vec2{0.0f})`.
 * */
std::pair<glm::vec2, glm::vec2> Block::GetSurfaceNormal(glm::vec2 origin,
                                                        glm::vec2 terminus) {
  origin = WorldToLocal(origin);
  terminus = WorldToLocal(terminus);
  std::pair<glm::vec2, glm::vec2> intersection =
      std::make_pair(glm::vec2{0.0f}, glm::vec2{0.0f});
  std::vector<std::pair<glm::vec2, glm::vec2>> intersections;
  if (GetHorizontalIntersection(origin, terminus, scale_.y, -scale_.x, scale_.x,
                                intersection)) {
    intersection.second = glm::vec2{0.0f, 1.0f};
    intersections.push_back(intersection);
  }
  if (GetHorizontalIntersection(origin, terminus, -scale_.y, -scale_.x,
                                scale_.x, intersection)) {
    intersection.second = glm::vec2{0.0f, -1.0f};
    intersections.push_back(intersection);
  }
  if (GetVerticalIntersection(origin, terminus, scale_.x, -scale_.y, scale_.y,
                              intersection)) {
    intersection.second = glm::vec2{1.0f, 0.0f};
    intersections.push_back(intersection);
  }
  if (GetVerticalIntersection(origin, terminus, -scale_.x, -scale_.y, scale_.y,
                              intersection)) {
    intersection.second = glm::vec2{-1.0f, 0.0f};
    intersections.push_back(intersection);
  }
  if (intersections.empty())
    return intersection;
  auto tmp = intersections.front();
  if (abs(terminus.x - origin.x) >= abs(terminus.y - origin.y)) {
    if (origin.x <= terminus.x)
      for (auto &intersection : intersections) {
        if (intersection.first.x < tmp.first.x)
          tmp = intersection;
      }
    else
      for (auto &intersection : intersections) {
        if (intersection.first.x > tmp.first.x)
          tmp = intersection;
      }
  } else {
    if (origin.y <= terminus.y)
      for (auto &intersection : intersections) {
        if (intersection.first.y < tmp.first.y)
          tmp = intersection;
      }
    else
      for (auto &intersection : intersections) {
        if (intersection.first.y > tmp.first.y)
          tmp = intersection;
      }
  }
  tmp.first = LocalToWorld(tmp.first);
  tmp.second = LocalToWorld(tmp.second) - LocalToWorld(glm::vec2{0.0f, 0.0f});
  return tmp;
}
}  // namespace battle_game::obstacle
