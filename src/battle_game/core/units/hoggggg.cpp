#include "hoggggg.h"

#include "battle_game/core/bullets/bullets.h"
#include "battle_game/core/game_core.h"
#include "battle_game/graphics/graphics.h"

#include <iostream>

namespace battle_game::unit {

namespace {
uint32_t hog_model_index = 0xffffffffu;
const float hog_PI = acos(-1);
} // namespace

Warthoggggg::Warthoggggg(GameCore *game_core, uint32_t id, uint32_t player_id) : Unit(game_core, id, player_id) {
    if (!~hog_model_index) {
        auto mgr = AssetsManager::GetInstance();

        std::vector<ObjectVertex> vertices;
        std::vector<uint32_t> indices;
        vertices.push_back({{0.0f, 0.3f}, {0.0f, 0.0f}, {0.7f, 0.7f, 0.7f, 1.0f}});
        vertices.push_back({{-0.7f, -1.0f}, {0.0f, 0.0f}, {0.7f, 0.7f, 0.7f, 1.0f}});
        vertices.push_back({{0.7f, -1.0f}, {0.0f, 0.0f}, {0.7f, 0.7f, 0.7f, 1.0f}});
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        vertices.push_back({{-0.1f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.7f, 0.7f, 1.0f}});
        vertices.push_back({{0.1f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.7f, 0.7f, 1.0f}});
        vertices.push_back({{-0.1f, 0.5f}, {0.0f, 0.0f}, {0.7f, 0.7f, 0.7f, 1.0f}});
        vertices.push_back({{0.1f, 0.5f}, {0.0f, 0.0f}, {0.7f, 0.7f, 0.7f, 1.0f}});
        indices.push_back(3 + 0);
        indices.push_back(3 + 1);
        indices.push_back(3 + 2);
        indices.push_back(3 + 1);
        indices.push_back(3 + 2);
        indices.push_back(3 + 3);
        hog_model_index = mgr->RegisterModel(vertices, indices);
    }
}

void Warthoggggg::Render() {
    battle_game::SetTransformation(position_, 0);
    battle_game::SetTexture(0);
    battle_game::SetColor(game_core_->GetPlayerColor(player_id_));
    battle_game::SetRotation(rotation_);
    battle_game::DrawModel(hog_model_index);
}

void Warthoggggg::Update() {
    _move(5.0f);
    _rotate(glm::radians(200.0f));
    _fire();
    // std::cerr << position_.x << ' ' << position_.y << ' ' << rotation_ << std::endl;
}

void Warthoggggg::_move(float move_speed) {
    auto player = game_core_->GetPlayer(player_id_);
    if (player) {
        glm::vec2 offset{0.0f, 1.0f};
        float speed = move_speed * GetSpeedScale();
        offset *= kSecondPerTick * speed;
        auto new_position = position_ + Rotate(offset, rotation_);
        if (!game_core_->IsBlockedByObstacles(new_position)) {
            game_core_->PushEventMoveUnit(id_, new_position);
        }
    }
}

void Warthoggggg::_rotate(float rotate_angular_speed) {
    auto player = game_core_->GetPlayer(player_id_);
    if (player) {
        auto &input_data = player->GetInputData();
        auto diff = input_data.mouse_cursor_position - position_;
        if (glm::length(diff) > 1e-4) {
            float rotation_range = kSecondPerTick * rotate_angular_speed * GetSpeedScale();
            float new_rotation = std::atan2(diff.y, diff.x) - glm::radians(90.0f) - rotation_;
            if (std::abs(new_rotation + 2 * hog_PI) < std::abs(new_rotation))
                new_rotation += 2 * hog_PI;
            if (std::abs(new_rotation - 2 * hog_PI) < std::abs(new_rotation))
                new_rotation -= 2 * hog_PI;
            new_rotation = std::max(-rotation_range, std::min(rotation_range, new_rotation));
            new_rotation += rotation_;
            game_core_->PushEventRotateUnit(id_, new_rotation);
        }
    }
}

void Warthoggggg::_fire() {
    if (fire_count_down_ == 0) {
        auto player = game_core_->GetPlayer(player_id_);
        if (player) {
            auto &input_data = player->GetInputData();
            if (input_data.mouse_button_down[GLFW_MOUSE_BUTTON_LEFT]) {
                auto velocity = Rotate(glm::vec2{0.0f, 20.0f}, rotation_);
                GenerateBullet<bullet::CannonBall>(position_ + Rotate({0.0f, 1.1f}, rotation_), rotation_,
                                                   GetDamageScale(), velocity, 2);
                fire_count_down_ = kTickPerSecond / 60; // Fire interval 1 second.
            }
        }
    }
    if (fire_count_down_) {
        fire_count_down_--;
    }
}

bool Warthoggggg::IsHit(glm::vec2 position) const {
    position = WorldToLocal(position);
    return position.x * position.x + position.y * position.y < 0.25;
}

const char *Warthoggggg::UnitName() const { return "Warthoggggg"; }

const char *Warthoggggg::Author() const { return "Zeyang Zhang"; }
} // namespace battle_game::unit