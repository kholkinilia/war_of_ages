#ifndef WAR_OF_AGES_SERVER_BULLET_H
#define WAR_OF_AGES_SERVER_BULLET_H

#include "bullet.h"

namespace war_of_ages::server {

std::shared_ptr<bullet> server_bullet_factory(bullet_type type, const vec2f &start, const vec2f &target);

struct server_bullet : bullet {
    server_bullet(bullet_type type, const vec2f &start, const vec2f &target);

    void post_create_action() const override;
    void post_collision_action() const override;
};

}  // namespace war_of_ages::server


#endif  // WAR_OF_AGES_SERVER_BULLET_H
