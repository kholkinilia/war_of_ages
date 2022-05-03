#ifndef WAR_OF_AGES_CONNECTION_H
#define WAR_OF_AGES_CONNECTION_H

#include <chrono>
#include <random>
#include "boost/asio.hpp"
#include "connection_fwd.h"
#include "message.h"
#include "server_fwd.h"
#include "thread_safe_deque.h"

namespace war_of_ages {

template <typename T>
struct connection : public std::enable_shared_from_this<connection<T>> {  // to make shared_ptr from this
private:
    void read_header() {
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(&m_receiving_message.header, sizeof(message_header<T>)),
                                [this](std::error_code ec, std::size_t length) {
                                    if (!ec) {
                                        if (m_receiving_message.header.size > 0) {
                                            m_receiving_message.data.resize(m_receiving_message.header.size);
                                            read_body();
                                        } else {
                                            add_to_received_messages();
                                        }
                                    } else {
                                        std::cout << "[" << m_id << "] Read header failed.\n";
                                        m_socket.close();
                                    }
                                });
    }

    void read_body() {
        boost::asio::async_read(
            m_socket, boost::asio::buffer(m_receiving_message.body.data(), m_receiving_message.header.size),
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    add_to_received_messages();
                } else {
                    std::cout << "[" << m_id << "] Read body failed.\n";
                    m_socket.close();
                }
            });
    }

    void write_header() {
        boost::asio::async_write(
            m_socket, boost::asio::buffer(&m_messages_to_send.front().header, sizeof(message_header<T>)),
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    if (m_messages_to_send.front().body.size() > 0) {
                        write_body();
                    } else {
                        m_messages_to_send.pop_front();

                        if (!m_messages_to_send.empty()) {
                            write_header();
                        }
                    };

                } else {
                    std::cout << "[" << m_id << "] Write header failed.\n";
                    m_socket.close();
                }
            });
    }

    void write_body() {
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(m_messages_to_send.front().body.data(),
                                                     m_messages_to_send.front().header.size),
                                 [this](std::error_code ec, std::size_t length) {
                                     if (!ec) {
                                         m_messages_to_send.pop_front();
                                         if (!m_messages_to_send.empty()) {
                                             write_header();
                                         }
                                     } else {
                                         std::cout << "[" << m_id << "] Write body failed.\n";
                                         m_socket.close();
                                     }
                                 });
    }

    void add_to_received_messages() {
        if (m_owner == owner::server) {
            m_messages_received.push_back(this->shared_from_this(), m_receiving_message);
        } else {
            m_messages_received.push_back(nullptr, m_receiving_message);
        }

        read_header();
    }

    std::uint64_t scramble(std::uint64_t input) {  // just a random function for em
        return (((input << 32) ^ (input >> 5)) * input) ^ 694201337;
    }

    void write_validation() {
        boost::asio::async_write(m_socket, boost::asio::buffer(&m_handshake_out, sizeof(std::uint64_t)),
                                 [this](std::error_code ec, std::size_t length) {
                                     if (!ec) {
                                         if (m_owner == owner::client) {
                                             read_header();
                                         }
                                     } else {
                                         m_socket.close();
                                     }
                                 });
    }

    void read_validation(server_inteface<T> *server = nullptr) {
        boost::asio::async_read(m_socket, boost::asio::buffer(&m_handshake_in, sizeof(std::uint64_t)),
                                [this, server](std::error_code ec, std::size_t length) {
                                    if (!ec) {
                                        if (m_owner == owner::server) {
                                            if (m_handshake_in == m_handshake_expected) {
                                                std::cout << "Client validated\n";
                                                server->on_client_validated(this->shared_from_this());
                                                read_header();
                                            } else {
                                                std::cout << "Client disconnected (failed validation)\n";
                                                m_socket.close();
                                            }
                                        } else {
                                            m_handshake_out = scramble(m_handshake_in);
                                            write_validation();
                                        }
                                    } else {
                                        std::cout << "Client disconnected (read_validation)\n";
                                        m_socket.close();
                                    };
                                });
    }

public:
    enum class owner { server, client };

    connection(owner parent,
               boost::asio::io_context &context,
               boost::asio::ip::tcp::socket socket,
               ts_deque<owned_message<T>> &messages_received)
        : m_owner(parent),
          m_context(context),
          m_socket(std::move(socket)),
          m_messages_received(messages_received) {
        if (m_owner == owner::server) {
            static std::mt19937_64 rnd(clock());
            m_handshake_out = rnd();
            m_handshake_expected = scramble(m_handshake_out);
        }
    }
    ~connection() = default;

    bool connect_to_server(const boost::asio::ip::tcp::resolver::results_type &endpoints) {
        if (m_owner == owner::client) {
            boost::asio::async_connect(
                m_socket, endpoints,
                [this](std::error_code ec, const boost::asio::ip::tcp::endpoint &endpoint) {
                    if (!ec) {
                        read_validation();
                    }
                });
        }
    }
    bool connect_to_client(server_inteface<T> *server, std::uint32_t client_id = 0) {
        if (m_owner == owner::server) {
            if (m_socket.is_open()) {
                m_id = client_id;
                write_validation();
                read_validation(server);
            }
        }
    }
    bool disconnect() {
        if (!is_connected()) {
            boost::asio::post(m_context, [this]() { m_socket.close(); });
        }
    }
    [[nodiscard]] bool is_connected() const {
        return m_socket.is_open();
    }

    bool send(const message<T> &msg) {
        boost::asio::post(m_context, [this, msg]() {
            bool is_sending = !m_messages_to_send.empty();
            m_messages_to_send.push_back(msg);
            if (!is_sending) {
                write_header();
            }
        });
    }

    [[nodiscard]] std::uint32_t get_id() const noexcept {
        return m_id;
    }

private:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_context &m_context;

    ts_deque<message<T>> m_messages_to_send;
    ts_deque<owned_message<T>> &m_messages_received;
    owner m_owner = owner::server;

    message<T> m_receiving_message;

    std::uint32_t m_id = 0;
    std::uint64_t m_handshake_out = 0;
    std::uint64_t m_handshake_in = 0;
    std::uint64_t m_handshake_expected = 0;
};

}  // namespace war_of_ages
#endif  // WAR_OF_AGES_CONNECTION_H
