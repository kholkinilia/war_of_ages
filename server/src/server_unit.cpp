#include "server_unit.h"

namespace war_of_ages::server {

void server_unit::post_create_action() const {
}

void server_unit::post_death_action() const {
}

void server_unit::post_attack_action() const {
}

server_unit::server_unit(unit_type type) : unit(type) {
}

std::shared_ptr<unit> server_unit_factory(unit_type type) {
    return std::make_shared<server_unit>(type);
}

}  // namespace war_of_ages::server
