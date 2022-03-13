#include "../include/game_commands.h"
#include "../include/player.h"

namespace war_of_ages {

void buy_unit_command::apply(player &p) {
    p.buy_unit(level);
}

void buy_cannon_command::apply(player &p) {
    p.buy_cannon(level, slot);
}

void sell_cannon_command::apply(player &p) {
    p.sell_cannon(slot);
}

void buy_cannon_slot_command::apply(player &p) {
    p.buy_cannon_slot();
}

void use_ult_command::apply(player &p) {
    p.use_ult();
}

void upgrade_age_command::apply(player &p) {
    p.upgrade_age();
}

// TODO: think of getting rid of constructors
buy_unit_command::buy_unit_command(int level_) : level(level_) {
}
buy_cannon_command::buy_cannon_command(int level_, int slot_) : level(level_), slot(slot_) {
}
sell_cannon_command::sell_cannon_command(int slot_) : slot(slot_) {
}

}  // namespace war_of_ages
