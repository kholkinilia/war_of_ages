#ifndef WAR_OF_AGES_SERVER_CANNON_H
#define WAR_OF_AGES_SERVER_CANNON_H

#include "cannon.h"

namespace war_of_ages::server {

std::shared_ptr<cannon> server_cannon_factory(cannon_type type, const vec2f &muzzle_position);

struct server_cannon : cannon {
    server_cannon(cannon_type type, const vec2f &muzzle_position);

    void post_buy_action() const override;
    void post_sell_action() const override;
};

}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_SERVER_CANNON_H
