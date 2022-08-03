#include "client_bullet.h"

namespace war_of_ages::client {

void client_bullet::post_collision_action() const {
}

void client_bullet::post_create_action(std::string owner_handle) const {
}

client_bullet::client_bullet(bullet_type type, const vec2f &start, const vec2f &target)
    : bullet(type, start, target) {
}

std::shared_ptr<bullet> client_bullet_factory(bullet_type type, const vec2f &start, const vec2f &target) {
    return std::make_shared<client_bullet>(type, start, target);
}

}  // namespace war_of_ages::client
