#include "../include/client.h"
#include "../include/bot_actions_supplier.h"

namespace war_of_ages::client {

client &client::instance() {
    static client inst;
    return inst;
}

client::client() : client_interface() {
    // TODO: read handle from local config.txt
    m_handle = "handle";
}

const std::string &client::get_handle() const noexcept {
    return m_handle;
}

}  // namespace war_of_ages::client
