#include "../include/game_handler.h"
#include "../include/bot_actions_receiver.h"
#include "../include/player_actions_receiver.h"

namespace war_of_ages {

game_handler::game_handler(std::vector<player_type> types_)
    : cur_game_state(std::make_shared<game_state>()), types(std::move(types_)) {
    receivers.resize(2);
    set_receiver(0, types[0]);
    set_receiver(1, types[1]);
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
void game_handler::set_receiver(int index, player_type type) {
    types[index] = type;
    switch (type) {
        case player_type::PLAYER:
            receivers[index] = std::make_shared<player_actions_receiver>();
            break;
        case player_type::BOT:
            receivers[index] = std::make_shared<bot_actions_receiver>();
            break;
    }
}
game_handler::player_type game_handler::get_type(int index) {
    return types.at(index);
}

}  // namespace war_of_ages