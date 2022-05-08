#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <mutex>
#include <string>
#include <vector>
#include "../../common/include/messages_type.h"
#include "../../common/network/include/client_interface.h"

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

    [[nodiscard]]  std::string get_handle() const;
    [[nodiscard]]  std::string get_password() const;

private:
    client();
    std::string m_handle;
    std::string m_password;
    mutable std::mutex m_mutex_handle_n_password;
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_CLIENT_H
