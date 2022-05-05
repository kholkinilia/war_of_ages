#include "../include/server.h"
#include <cassert>

namespace war_of_ages {

server::server(std::uint16_t port) : server_interface<messages_type>(port) {
}

void server::send_message(const std::string &handle, const message<messages_type> &msg) {
    auto it_id = m_id_by_handle.find(handle);
    assert(it_id != m_id_by_handle.end());
    auto it_connection = m_connection_by_id.find(it_id->second);
    assert(it_connection != m_connection_by_id.end());
    server_interface::send_message(it_connection->second, msg);
}

bool server::on_client_connect(std::shared_ptr<connection<messages_type>> client) {
    m_connection_by_id.insert({client->get_id(), client});
    std::unique_lock l(m_mutex_status);
    m_status_by_id.insert({client->get_id(), user_status::MENU});
    return true;
}

void server::on_client_disconnect(std::shared_ptr<connection<messages_type>> client) {
    std::uint32_t id = client->get_id();
    m_connection_by_id.erase(id);
    auto handle_it = m_handle_by_id.find(id);
    if (handle_it != m_handle_by_id.end()) {
        m_id_by_handle.erase(handle_it->second);
        m_handle_by_id.erase(handle_it);
    }
    std::unique_lock l(m_mutex_status);
    m_status_by_id.erase(id);
    // TODO: tournament & matches logic
}

void server::on_message(std::shared_ptr<connection<messages_type>> client,
                        const message<messages_type> &msg) {
    // TODO: implement
}

}  // namespace war_of_ages