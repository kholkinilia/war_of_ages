#ifndef WAR_OF_AGES_NET_SERVER_H
#define WAR_OF_AGES_NET_SERVER_H

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include "../../common/include/messages_type.h"
#include "../../common/network/include/server_interface.h"
#include "../include/room_matchmaker.h"

namespace war_of_ages {

enum class user_status {
    AUTHORIZATION,
    MENU,
    RANDOMGAME,
    ROOM,
    TOURNAMENT,
};

struct server final : server_interface<messages_type> {
public:
    explicit server(std::uint16_t port);
    void send_message(const std::string &handle, const message<messages_type> &msg);

private:
    std::unordered_map<std::uint32_t, std::shared_ptr<connection<messages_type>>> m_connection_by_id;
    std::unordered_map<std::uint32_t, std::string> m_handle_by_id;
    std::unordered_map<std::string, std::uint32_t> m_id_by_handle;
    std::unordered_map<std::uint32_t, user_status> m_status_by_id;
    std::mutex m_mutex_status;

protected:
    bool on_client_connect(std::shared_ptr<connection<messages_type>> client) override;
    void on_client_disconnect(std::shared_ptr<connection<messages_type>> client) override;
    void on_message(std::shared_ptr<connection<messages_type>> client, message<messages_type> msg) override;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_NET_SERVER_H
