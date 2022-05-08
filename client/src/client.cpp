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

std::vector<owned_message<messages_type>> client::retrieve_messages() {
    return get_messages_received().retrieve();
}

void client::clear_messages() {
    get_messages_received().clear();
}

void client::login() {
    message<messages_type> msg;
    msg.header.id = messages_type::AUTH_LOGIN;
    std::unique_lock l(m_mutex_handle_n_password);
    msg << m_handle << m_password;
    send_message(msg);
}

void client::ignore_server() {
    message<messages_type> msg;
    msg.header.id = messages_type::SERVER_IGNORE;
    send_message(msg);
}

void client::set_handle(std::string handle) {
    std::unique_lock l(m_mutex_handle_n_password);
    m_handle = std::move(handle);
}

void client::set_password(std::string password) {
    std::unique_lock l(m_mutex_handle_n_password);
    m_password = std::move(password);
}

std::string client::get_handle() const {
    std::unique_lock l(m_mutex_handle_n_password);
    return m_handle;
}

std::string client::get_password() const {
    std::unique_lock l(m_mutex_handle_n_password);
    return m_password;
}

}  // namespace war_of_ages::client
