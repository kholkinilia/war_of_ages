#include "client_cannon.h"

namespace war_of_ages::client {

void client_cannon::post_buy_action() const {
}

void client_cannon::post_sell_action() const {
}

client_cannon::client_cannon(cannon_type type, const vec2f &muzzle_position) : cannon(type, muzzle_position) {
}

std::shared_ptr<cannon> client_cannon_factory(cannon_type type, const vec2f &muzzle_position) {
    return std::make_shared<client_cannon>(type, muzzle_position);
}

}  // namespace war_of_ages::client
