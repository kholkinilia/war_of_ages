#ifndef WAR_OF_AGES_CLIENT_CANNON_H
#define WAR_OF_AGES_CLIENT_CANNON_H

#include "cannon.h"

namespace war_of_ages::client {

std::shared_ptr<cannon> client_cannon_factory(cannon_type type, const vec2f &muzzle_position);

struct client_cannon : cannon {
    client_cannon(cannon_type type, const vec2f &muzzle_position);

    void post_buy_action() const override;
    void post_sell_action() const override;
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_CLIENT_CANNON_H
