#include "../include/game_handler.h"

namespace war_of_ages {

game_handler::game_handler(std::vector<std::shared_ptr<actions_receiver>> receivers_)
    : cur_game_state(std::make_shared<game_state>(1.f * clock() / CLOCKS_PER_SEC)),
      receivers(std::move(receivers_)) {
}
std::shared_ptr<game_state> game_handler::get_cur_game_state() const {
    return cur_game_state;
}
void game_handler::reset() {
    cur_game_state = nullptr;
}
std::vector<std::unique_ptr<game_command>> const &game_handler::get_actions(int index, player_snapshot p) {
    return receivers[index]->get_actions(std::move(p));
}
void game_handler::append_action(int index, std::unique_ptr<game_command> cmd) {
    receivers[index]->append(std::move(cmd));
}
void game_handler::clear_actions() {
    receivers[0]->clear();
    receivers[1]->clear();
}

}  // namespace war_of_ages