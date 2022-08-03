#ifndef WAR_OF_AGES_CLIENT_BULLET_H
#define WAR_OF_AGES_CLIENT_BULLET_H

#include "bullet.h"

namespace war_of_ages::client {

std::shared_ptr<bullet> client_bullet_factory(bullet_type type, const vec2f &start, const vec2f &target);

struct client_bullet : bullet {
    client_bullet(bullet_type type, const vec2f &start, const vec2f &target);

    void post_create_action(std::string owner_handle) const override;
    void post_collision_action() const override;
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_CLIENT_BULLET_H
