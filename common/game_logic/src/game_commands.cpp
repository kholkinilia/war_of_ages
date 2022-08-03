#include "game_commands.h"
#include "player.h"

namespace war_of_ages {

bool buy_unit_command::apply(player &p) {
    return p.buy_unit(level);
}

bool buy_cannon_command::apply(player &p) {
    return p.buy_cannon(level, slot);
}

bool sell_cannon_command::apply(player &p) {
    return p.sell_cannon(slot);
}

bool buy_cannon_slot_command::apply(player &p) {
    return p.buy_cannon_slot();
}

bool use_ult_command::apply(player &p) {
    return p.use_ult();
}

bool upgrade_age_command::apply(player &p) {
    return p.upgrade_age();
}

bool add_bullet_command::apply(player &p) {
    return p.add_bullet(snapshot);
}

// TODO: think of getting rid of constructors
buy_unit_command::buy_unit_command(int level_) : level(level_) {
}
buy_cannon_command::buy_cannon_command(int level_, int slot_) : level(level_), slot(slot_) {
}
sell_cannon_command::sell_cannon_command(int slot_) : slot(slot_) {
}
add_bullet_command::add_bullet_command(bullet_snapshot snapshot_) : snapshot(snapshot_) {
}

}  // namespace war_of_ages
