#ifndef WAR_OF_AGES_SERVER_UNIT_H
#define WAR_OF_AGES_SERVER_UNIT_H

#include "unit.h"
namespace war_of_ages::server {

std::shared_ptr<unit> server_unit_factory(unit_type type);

struct server_unit : unit {
    explicit server_unit(unit_type type);

    void post_attack_action() const override;
    void post_create_action() const override;
    void post_death_action() const override;
};

}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_SERVER_UNIT_H
