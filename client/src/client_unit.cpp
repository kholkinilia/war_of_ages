#include "client_unit.h"

namespace war_of_ages::client {

void client_unit::post_create_action() const {
}

void client_unit::post_death_action() const {
}

void client_unit::post_attack_action() const {
}

client_unit::client_unit(unit_type type) : unit(type) {
}

std::shared_ptr<unit> client_unit_factory(unit_type type) {
    return std::make_shared<client_unit>(type);
}

}  // namespace war_of_ages::client
