#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <string>
#include "../../common/include/messages_type.h"
#include "../../common/network/include/client_interface.h"

namespace war_of_ages::client {

// Will be implemented to the end by Timur

struct client final : client_interface<messages_type> {
public:
    [[nodiscard]] static client &instance();
    ~client() final = default;
    client(const client &other) = delete;
    client(client &&other) = delete;
    client &operator=(const client &other) = delete;
    client &operator=(client &&other) = delete;

    [[nodiscard]] const std::string &get_handle() const noexcept;

private:
    client();
    std::string m_handle;
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_CLIENT_H
