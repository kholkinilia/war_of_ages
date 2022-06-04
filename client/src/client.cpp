#include "../include/client.h"
#include <fstream>
#include <iostream>
#include "../include/application.h"
#include "../include/bot_actions_supplier.h"

namespace war_of_ages::client {

client &client::instance() {
    static client inst;
    return inst;
}

client::client() : client_interface() {
    std::fstream file("../config.txt");

    std::getline(file, m_server_ip);
    std::string server_port_string;
    std::getline(file, server_port_string);
    m_server_port = std::stoi(server_port_string);

    std::ifstream login_password("../client/configs/client_config.txt");
    std::getline(login_password, m_handle);
    std::getline(login_password, m_password);
}

std::vector<owned_message<messages_type>> client::retrieve_messages() {
    return get_messages_received().retrieve();
}

void client::clear_messages() {
    get_messages_received().clear();
}

void client::login_or_authorize(bool is_login) {
    message<messages_type> msg;
    msg.header.id = is_login ? messages_type::AUTH_LOGIN : messages_type::AUTH_REGISTER;
    std::unique_lock l(m_mutex_handle_n_password);
    msg.insert_container(m_handle);
    msg.insert_container(m_password);
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

std::string client::get_server_ip() const {
    std::unique_lock l(m_mutex_handle_n_password);
    return m_server_ip;
}

std::uint16_t client::get_server_port() const {
    std::unique_lock l(m_mutex_handle_n_password);
    return m_server_port;
}

bool client::get_is_authorized() const {
    return m_is_authorized;
}

void client::set_is_authorized(bool value) {
    m_is_authorized = value;
}
void client::ask_for_stats() {
    message<messages_type> msg;
    msg.header.id = messages_type::STATS_GET;
    std::unique_lock l(m_mutex_stats);
    send_message(msg);
}

}  // namespace war_of_ages::client
