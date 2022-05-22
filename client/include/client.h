#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <mutex>
#include <string>
#include <vector>
#include "messages_type.h"
#include "client_interface.h"

namespace war_of_ages::client {
struct client final : client_interface<messages_type> {
public:
    [[nodiscard]] static client &instance();
    ~client() final = default;
    client(const client &other) = delete;
    client(client &&other) = delete;
    client &operator=(const client &other) = delete;
    client &operator=(client &&other) = delete;

    void clear_messages();
    [[nodiscard]] std::vector<owned_message<messages_type>> retrieve_messages();

    void login();
    void ignore_server();
    void set_handle(std::string handle);
    void set_password(std::string password);

    [[nodiscard]] std::string get_handle() const;
    [[nodiscard]] std::string get_password() const;
    [[nodiscard]] std::string get_server_ip() const;
    [[nodiscard]] std::uint16_t get_server_port() const;

private:
    client();
    std::string m_handle;
    std::string m_password;
    std::string m_server_ip;
    std::uint16_t m_server_port;
    mutable std::mutex m_mutex_handle_n_password;
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_CLIENT_H
