#ifndef WAR_OF_AGES_CLIENT_UNIT_H
#define WAR_OF_AGES_CLIENT_UNIT_H

#include "unit.h"
namespace war_of_ages::client {

std::shared_ptr<unit> client_unit_factory(unit_type type);

struct client_unit : unit {
    explicit client_unit(unit_type type);

    void post_attack_action() const override;
    void post_create_action() const override;
    void post_death_action() const override;
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_CLIENT_UNIT_H
