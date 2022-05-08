#include "../include/client.h"
#include "../include/bot_actions_supplier.h"

namespace war_of_ages {

client_state::client_state(std::string handle_) : handle(std::move(handle_)) {
}

std::string client_state::get_handle() const {
    return handle;
}

std::shared_ptr<tournament_handler> client_state::get_cur_tournament() const {
    return cur_tournament;
}

}  // namespace war_of_ages
