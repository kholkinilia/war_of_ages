#ifndef WAR_OF_AGES_CLIENT_INTERFACE_H
#define WAR_OF_AGES_CLIENT_INTERFACE_H

#include <string>
#include <thread>
#include "connection.h"
#include "message.h"
#include "thread_safe_deque.h"

namespace war_of_ages {

template <typename T>
struct client_interface {
private:
    ts_deque<owned_message<T>> m_messages_received;

protected:
    boost::asio::io_context m_context;
    std::thread context_thread;
    boost::asio::ip::tcp::socket m_socket;
    std::unique_ptr<connection<T>> m_connection;

public:
    virtual ~client_interface() {
        disconnect();
    }

    bool connect(const std::string &host, const uint16_t port) {
        try {
            boost::asio::ip::tcp::resolver resolver(m_context);
            boost::asio::ip::tcp::resolver::results_type endpoints =
                resolver.resolve(host, std::to_string(port));

            m_connection =
                std::make_unique<connection<T>>(connection<T>::owner::client, m_context,
                                                boost::asio::ip::tcp::socket(m_context), m_messages_received);

            m_connection->connect_to_server(endpoints);

            std::thread([this]() { m_context.run(); }).detach();

        } catch (std::exception &e) {
            std::cerr << "Client exception: " << e.what() << "\n";
            return false;
        }
        return true;
    };

    void disconnect() {
        if (is_connected()) {
            m_connection->disconnect();
        }
    }

    bool is_connected() {
        if (m_connection) {
            return m_connection->is_connected();
        }
        return false;
    }

    ts_deque<owned_message<T>> &get_messages_received() {
        return m_messages_received;
    }
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CLIENT_INTERFACE_H
