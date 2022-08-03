#ifndef WAR_OF_AGES_GAME_COMMANDS_H
#define WAR_OF_AGES_GAME_COMMANDS_H

#include <vector>
#include "player.h"

namespace war_of_ages {

struct game_command {
    virtual bool apply(player &p) = 0;
    virtual ~game_command() = default;
};

struct buy_unit_command : game_command {
private:
    int level;

public:
    explicit buy_unit_command(int level_);
    bool apply(player &p) final;
};

struct buy_cannon_command : game_command {
private:
    int level;
    int slot;

public:
    explicit buy_cannon_command(int level_, int slot_);
    bool apply(player &p) final;
};

struct sell_cannon_command : game_command {
private:
    int slot;

public:
    explicit sell_cannon_command(int slot_);
    bool apply(player &p) final;
};

struct buy_cannon_slot_command : game_command {
public:
    bool apply(player &p) final;
};

struct use_ult_command : game_command {
public:
    bool apply(player &p) final;
};

struct upgrade_age_command : game_command {
public:
    bool apply(player &p) final;
};

struct add_bullet_command : game_command {
private:
    bullet_snapshot snapshot;
public:
    explicit add_bullet_command(bullet_snapshot);
    bool apply(player &p) final;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_COMMANDS_H
