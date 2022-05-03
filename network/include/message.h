#ifndef WAR_OF_AGES_MESSAGE_H
#define WAR_OF_AGES_MESSAGE_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>
#include "connection_fwd.h"

namespace war_of_ages {

/// Works as stack: you need to extract elements in reversed order.
/// For non-trivially copyable types such as string use existing functions (i.e. insert_container()) or
/// implement ones.
/// Note that all sizes stored in message are represented by std::uint32_t.

template <typename T>
struct message_header {
    T id{};
    std::uint32_t size = 0;
};

template <typename T>
struct message {
    message_header<T> header;
    std::vector<std::uint8_t> body;

    explicit message(T id) : header(id){};

    [[nodiscard]] std::size_t size() const noexcept {
        return body.size();
    }

    friend std::ostream &operator<<(std::ostream &out, const message &msg) {  // in case of debugging
        out << "Message (id=" << msg.header.id << "): size=" << msg.size() << "\n";
        return out;
    }

    template <typename DataType>
    friend message &operator<<(message &msg, const DataType &src) {
        static_assert(std::is_trivially_copyable_v<DataType>,
                      "Data you want to pass to a message is not trivially copyable.");
        std::uint32_t insertion_pos = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(src));
        memcpy(msg.body.data() + insertion_pos, reinterpret_cast<const std::uint8_t *>(&src), sizeof(src));
        msg.header.size = msg.size();
        return msg;
    }

    template <typename DataType>
    friend message &operator>>(message &msg, DataType &dst) {
        static_assert(std::is_trivially_copyable_v<DataType>,
                      "Data you want to extract from a message is not trivially copyable.");
        assert(msg.body.size() >= sizeof(dst));
        std::uint32_t extraction_pos = msg.body.size() - sizeof(dst);
        memcpy(reinterpret_cast<std::uint8_t *>(&dst), msg.body.data() + extraction_pos, sizeof(dst));
        msg.body.resize(extraction_pos);
        msg.header.size = msg.size();
        return msg;
    }

    void insert_buf(const std::uint8_t *buf, std::uint32_t number_of_bytes) {
        std::uint32_t insertion_pos = body.size();
        body.resize(body.size() + number_of_bytes);
        memcpy(body.data() + insertion_pos, buf, number_of_bytes);
        header.size = size();
    }

    void extract_buf(std::uint8_t *buf, std::uint32_t number_of_bytes) {
        assert(body.size() >= number_of_bytes);
        std::uint32_t extraction_pos = body.size() - number_of_bytes;
        memcpy(buf, body.data() + extraction_pos, number_of_bytes);
        body.resize(extraction_pos);
        header.size = size();
    }

    template <typename ContainerType>
    void insert_container(const ContainerType &container) {
        static_assert(std::is_trivially_copyable_v<typename ContainerType::value_type>,
                      "Data stored in container is not trivially copyable.");
        std::uint32_t value_size = sizeof(typename ContainerType::value_type);
        std::uint32_t container_size = container.size();
        insert_buf(reinterpret_cast<const std::uint8_t *>(container.data()),
                   value_size * container.size());  // TODO: check if copying entire container.data() is ok
        *this << container_size;
    }

    template <typename ContainerType>
    void extract_container(ContainerType &container) {
        static_assert(std::is_trivially_copyable_v<typename ContainerType::value_type>,
                      "Data stored in container is not trivially copyable.");
        std::uint32_t value_size = sizeof(typename ContainerType::value_type);
        std::uint32_t container_size;
        *this >> container_size;
        container.resize(container_size, typename ContainerType::value_type{});
        extract_buf(reinterpret_cast<std::uint8_t *>(container.data()),
                    container_size * value_size);  // TODO: check if copying entire container.data() is ok
    }
};

template <typename T>
struct owned_message {
    std::shared_ptr<connection<T>> remote = nullptr;
    message<T> msg;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_MESSAGE_H
