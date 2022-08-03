#include "server_bullet.h"

namespace war_of_ages::server {

void server_bullet::post_collision_action() const {
}

void server_bullet::post_create_action() const {
}

server_bullet::server_bullet(bullet_type type, const vec2f &start, const vec2f &target)
    : bullet(type, start, target) {
}

std::shared_ptr<bullet> server_bullet_factory(bullet_type type, const vec2f &start, const vec2f &target) {
    return std::make_shared<server_bullet>(type, start, target);
}

}  // namespace war_of_ages::server
