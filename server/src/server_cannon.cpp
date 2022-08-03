#include "server_cannon.h"

namespace war_of_ages::server {

void server_cannon::post_buy_action() const {
}

void server_cannon::post_sell_action() const {
}

server_cannon::server_cannon(cannon_type type, const vec2f &muzzle_position)
    : cannon(type, muzzle_position) {
}

std::shared_ptr<cannon> server_cannon_factory(cannon_type type, const vec2f &muzzle_position) {
    return std::make_shared<server_cannon>(type, muzzle_position);
}

}  // namespace war_of_ages::server
