#include "game_state.h"
#include <memory>
#include <vector>

namespace war_of_ages {

void game_state::update(const std::vector<std::unique_ptr<game_command>> &p1_commands,
                        const std::vector<std::unique_ptr<game_command>> &p2_commands) {
    std::uint64_t time = get_ms();
    p1.update(p2, static_cast<float>(time - m_state_time) / MS_PER_SEC);
    p2.update(p1, static_cast<float>(time - m_state_time) / MS_PER_SEC);

    p1.berserk_units(p2);
    p2.berserk_units(p1);

    p1.collect_profit(p2);
    p2.collect_profit(p1);

    p1.clear_dead_objects();
    p2.clear_dead_objects();

    m_state_time = time;

    for (auto &command : p1_commands) {
        command->apply(p1);
    }
    for (auto &command : p2_commands) {
        command->apply(p2);
    }
}

std::pair<player_snapshot, player_snapshot> game_state::snapshot_players() const {
    return {p1.snapshot(), p2.snapshot()};
}

game_state::game_state(
    const std::function<std::shared_ptr<unit>(unit_type)> &unit_factory,
    const std::function<std::shared_ptr<bullet>(bullet_type, const vec2f &, const vec2f &)> &bullet_factory,
    const std::function<std::shared_ptr<cannon>(cannon_type, const vec2f &)> &cannon_factory)
    : m_state_time(get_ms()),
      m_start_time(m_state_time),
      p1(unit_factory, bullet_factory, cannon_factory),
      p2(unit_factory, bullet_factory, cannon_factory) {
}

game_status game_state::get_game_status() const {
    if (!p1.is_alive()) {
        return game_status::P2_WON;
    }
    if (!p2.is_alive()) {
        return game_status::P1_WON;
    }
    return game_status::PROCESSING;
}

void game_state::return_from_pause() noexcept {
    m_state_time = get_ms();
}

void game_state::set_state(const player_snapshot &p1_snap, const player_snapshot &p2_snap) {
    p1.set_snapshot(p1_snap);
    p2.set_snapshot(p2_snap);
}

game_state::game_state(
    std::uint64_t start_time,
    const std::function<std::shared_ptr<unit>(unit_type)> &unit_factory,
    const std::function<std::shared_ptr<bullet>(bullet_type, const vec2f &, const vec2f &)> &bullet_factory,
    const std::function<std::shared_ptr<cannon>(cannon_type, const vec2f &)> &cannon_factory)
    : m_state_time(get_ms()),
      m_start_time(start_time),
      p1(unit_factory, bullet_factory, cannon_factory),
      p2(unit_factory, bullet_factory, cannon_factory) {
}

std::uint64_t game_state::get_start_time() const noexcept {
    return m_state_time;
}

bool game_state::apply_command(game_state::side player_side, std::unique_ptr<game_command> cmd) {
    if (player_side == side::FIRST) {
        return cmd->apply(p1);
    }
    return cmd->apply(p2);
}

}  // namespace war_of_ages
