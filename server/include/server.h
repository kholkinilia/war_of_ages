#ifndef WAR_OF_AGES_NET_SERVER_H
#define WAR_OF_AGES_NET_SERVER_H

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include "../../common/include/messages_type.h"
#include "../../common/network/include/server_interface.h"

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
    [[nodiscard]] static server &instance();
    ~server() final = default;
    server(const server &other) = delete;
    server(server &&other) = delete;
    server &operator=(const server &other) = delete;
    server &operator=(server &&other) = delete;

    static void set_port(std::uint16_t port);

    void send_message(const std::string &handle, const message<messages_type> &msg);
    [[nodiscard]] user_status get_user_status(std::uint32_t user_id) const;

private:
    server();
    static inline std::uint16_t m_port = 0;

    void set_user_status(std::uint32_t user_id, user_status new_status);

    std::unordered_map<std::uint32_t, std::shared_ptr<connection<messages_type>>> m_connection_by_id;
    std::unordered_map<std::uint32_t, std::string> m_handle_by_id;
    std::unordered_map<std::string, std::uint32_t> m_id_by_handle;
    std::unordered_map<std::uint32_t, user_status> m_status_by_id;
    mutable std::mutex m_mutex_status;

protected:
    bool on_client_connect(std::shared_ptr<connection<messages_type>> client) override;
    void on_client_disconnect(std::shared_ptr<connection<messages_type>> client) override;
    void on_message(std::shared_ptr<connection<messages_type>> client, message<messages_type> msg) override;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_NET_SERVER_H
