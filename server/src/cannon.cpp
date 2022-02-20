#include "../include/cannon.h"
#include "../include/bullet.h"

namespace war_of_ages {

// TODO: implement struct cannon

cannon_stats::cannon_stats(int cost_, int cooldown_) {
}

cannon::cannon(cannon_type type, int x, int y) {
}

void cannon::update(unit &enemy, int dt) {
}

cannon_type cannon::type() {
    return cannon_type::NONE;
}

int cannon::x() {
    return 0;
}

int cannon::y() {
    return 0;
}

cannon_stats cannon::get_stats(cannon_type type) {
    return cannon_stats(0, 0);
}

}  // namespace war_of_ages
