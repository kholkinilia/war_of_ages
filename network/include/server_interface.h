#ifndef WAR_OF_AGES_SERVER_INTERFACE_H
#define WAR_OF_AGES_SERVER_INTERFACE_H

#include <algorithm>
#include <thread>
#include <unordered_map>
#include "connection.h"
#include "message.h"
#include "thread_safe_deque.h"

namespace war_of_ages {

template <typename T>
struct server_interface {
public:
    explicit server_interface(std::uint16_t port)
        : m_acceptor(m_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)){};

    virtual ~server_interface() {
        stop();
    }

    bool start() {
        try {
            wait_for_connection();

            m_context_thread = std::thread([this]() { m_context.run(); });
        } catch (std::exception &e) {
            std::cerr << "[SERVER] exception: " << e.what() << ".\n";
            return false;
        }
        std::cout << "[SERVER] started.\n";
        return true;
    }

    bool stop() {
        m_context.stop();

        if (m_context_thread.joinable()) {
            m_context_thread.join();
        }

        std::cout << "[SERVER] stopped.\n";
        return true;
    }

    void wait_for_connection() {  // async
        m_acceptor.async_accept([this](std::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::cout << "[SERVER] New connection: " << socket.remote_endpoint() << ".\n";

                std::shared_ptr<connection<T>> new_connection = std::make_shared<connection<T>>(
                    connection<T>::owned::server, m_context, std::move(socket), m_messages_received);

                if (on_client_connect(new_connection)) {
                    m_active_connections.push_back(std::move(new_connection));
                    m_active_connections.back()->connect_to_client(this, m_id_counter++);

                    std::cout << "[" << m_active_connections.back().get_id() << "] Connection approved.\n";
                } else {
                    std::cout << "[------] Connection denied.\n";
                }
            } else {
                std::cout << "[SERVER] Connection error: " << ec.message() << ".\n";
            }
            wait_for_connection();
        });
    }

    void send_message(std::shared_ptr<connection<T>> client, const message<T> &msg) {
        if (client && client->is_connected()) {
            client->send(msg);
        } else {
            on_client_disconnect(client);
            client.reset();
            m_active_connections.erase(
                std::remove(m_active_connections.begin(), m_active_connections.end(), client),
                m_active_connections.end());
        }
    }

    void message_all_clients(const message<T> &msg,
                             std::shared_ptr<connection<T>> client_to_ignore = nullptr) {
        bool dead_clients_exist = false;
        for (auto &client : m_active_connections) {
            if (client && client->is_connected()) {
                if (client != client_to_ignore) {
                    client->send_message(msg);
                }
            } else {
                on_client_disconnect(client);
                client.reset();
                dead_clients_exist = true;
            }
        }
        if (dead_clients_exist) {
            m_active_connections(
                std::remove(m_active_connections.begin(), m_active_connections.end(), nullptr),
                m_active_connections.end());
        }
    }

    void update(std::size_t number_of_messages = -1, bool wait_in_between = false) {
        if (wait_in_between) {
            m_messages_received.wait();
        }
        std::size_t messages_processed = 0;
        while (messages_processed < number_of_messages && !m_messages_received.empty()) {
            auto msg = m_messages_received.pop_front();
            on_message(msg.remote, msg.msg);
            messages_processed++;
        }
    }

protected:
    ts_deque<owned_message<T>> m_messages_received;
    boost::asio::io_context m_context;
    std::thread m_context_thread;
    boost::asio::ip::tcp::acceptor m_acceptor;

    std::deque<std::shared_ptr<connection<T>>> m_active_connections;
    int m_id_counter = 10000;

    virtual bool on_client_connect(std::shared_ptr<connection<T>> client) = 0;
    virtual bool on_client_disconnect(std::shared_ptr<connection<T>> client) = 0;
    virtual void on_message(std::shared_ptr<connection<T>> client, const message<T> &msg) = 0;
    virtual void on_client_validated(std::shared_ptr<connection<T>> client) = 0;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SERVER_INTERFACE_H
