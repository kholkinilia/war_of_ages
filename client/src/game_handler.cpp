#include "../include/game_handler.h"
#include "../include/bot_actions_supplier.h"
#include "../include/player_actions_supplier.h"

namespace war_of_ages {

game_handler::game_handler(std::vector<player_type> types_)
    : cur_game_state(std::make_shared<game_state>(1.f * clock() / CLOCKS_PER_SEC)), types(std::move(types_)) {
    suppliers.resize(2);
    set_receiver(0, types[0]);
    set_receiver(1, types[1]);
}
std::shared_ptr<game_state> game_handler::get_cur_game_state() const {
    return cur_game_state;
}
void game_handler::reset() {
    cur_game_state = nullptr;
}
std::vector<std::unique_ptr<game_command>> game_handler::get_actions(int index) {
    return suppliers[index]->pop_actions();
}
void game_handler::append_action(int index, std::unique_ptr<game_command> cmd) {
    suppliers[index]->add_action(std::move(cmd));
}
void game_handler::clear_actions() {
    suppliers[0]->clear_actions();
    suppliers[1]->clear_actions();
}
void game_handler::set_receiver(int index, player_type type) {
    types[index] = type;
    switch (type) {
        case player_type::PLAYER:
            suppliers[index] = std::make_shared<player_actions_supplier>();
            break;
        case player_type::BOT:
            suppliers[index] = std::make_shared<bot_actions_supplier>();
            break;
    }
}
game_handler::player_type game_handler::get_type(int index) {
    return types.at(index);
}

}  // namespace war_of_ages