#ifndef WAR_OF_AGES_CONNECTION_H
#define WAR_OF_AGES_CONNECTION_H

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include "connection_fwd.h"
#include "message.h"
#include "server_fwd.h"
#include "thread_safe_deque.h"

namespace war_of_ages {

template <typename T>
struct connection : public std::enable_shared_from_this<connection<T>> {  // to make shared_ptr from this
public:
    void read_header() {
        boost::asio::async_read(
            m_socket, boost::asio::buffer(&m_receiving_message.header, sizeof(message_header<T>)),
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    if (m_receiving_message.header.size > 0) {
                        if (m_receiving_message.header.size > /* just some upper bound */ 100) {
                            std::cout << "[" << m_id << "] Received very big message ("
                                      << m_receiving_message.header.size << " bytes)." << std::endl;
                            m_socket.close();
                            return;
                        }
                        m_receiving_message.body.resize(m_receiving_message.header.size);
                        read_body();
                    } else {
                        add_to_received_messages();
                    }
                } else {
                    std::cout << "[" << m_id << "] Read header failed." << std::endl;
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
                    std::cout << "[" << m_id << "] Read body failed." << std::endl;
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
                    std::cout << "[" << m_id << "] Write header failed.\nwith message:" << std::endl;
                    std::cout << ec.message();
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
                                         std::cout << "[" << m_id << "] Write body failed." << std::endl;
                                         m_socket.close();
                                     }
                                 });
    }

    void add_to_received_messages() {
        if (m_owner == owner::server) {
            m_messages_received.push_back({this->shared_from_this(), m_receiving_message});
        } else {
            m_messages_received.push_back({nullptr, m_receiving_message});
        }

        read_header();
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
          m_messages_received(messages_received){};

    connection(const connection &other) = delete;
    connection(connection &&other) noexcept = default;
    connection &operator=(const connection &other) = delete;
    connection &operator=(connection &&other) = default;

    ~connection() {
        disconnect();
    }

    void connect_to_server(const boost::asio::ip::tcp::resolver::results_type &endpoints) {
        if (m_owner == owner::client) {
            boost::asio::async_connect(
                m_socket, endpoints,
                [this](std::error_code ec, const boost::asio::ip::tcp::endpoint &endpoint) {
                    if (!ec) {
                        read_header();
                    }
                });
        }
    }
    void connect_to_client(server_interface<T> *server, std::uint32_t client_id = 0) {
        if (m_owner == owner::server) {
            if (m_socket.is_open()) {
                m_id = client_id;
                read_header();
            }
        }
    }
    void disconnect() {
        if (!is_connected()) {
            boost::asio::post(m_context, [this]() { m_socket.close(); });
        }
    }
    [[nodiscard]] bool is_connected() const {
        return m_socket.is_open();
    }

    void send(const message<T> &msg) {
        boost::asio::post(m_context, [this, msg]() {
            bool is_sending = !m_messages_to_send.empty();
            m_messages_to_send.push_back(msg);
            if (!is_sending) {
                std::cout << "Queue was empty, started new write_header()" << std::endl;
                write_header();
            }
        });
    }

    [[nodiscard]] std::uint32_t get_id() const noexcept {
        return m_id;
    }

public:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_context &m_context;

    ts_deque<message<T>> m_messages_to_send;
    ts_deque<owned_message<T>> &m_messages_received;
    owner m_owner = owner::server;

    message<T> m_receiving_message;

    std::uint32_t m_id = 0;
};

}  // namespace war_of_ages
#endif  // WAR_OF_AGES_CONNECTION_H
