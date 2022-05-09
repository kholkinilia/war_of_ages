#ifndef WAR_OF_AGES_MESSAGE_H
#define WAR_OF_AGES_MESSAGE_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
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

    [[nodiscard]] std::size_t size() const noexcept {
        return body.size();
    }

    friend std::ostream &operator<<(std::ostream &out, const message &msg) {  // in case of debugging
        out << "Message (m_id=" << (int)msg.header.id << "): size=" << msg.size() << "\n";
        return out;
    }

    template <typename DataType>
    friend message &operator<<(message &msg, const DataType &src) {
        if (!std::is_trivially_copyable_v<DataType>) {
            std::cerr << "Data you want to pass to a message is not trivially copyable.";
            assert(false);
        }
        msg.insert_buf(reinterpret_cast<const std::uint8_t *>(&src), sizeof(DataType));
        return msg;
    }

    template <typename DataType>
    friend message &operator>>(message &msg, DataType &dst) {
        if (!std::is_trivially_copyable_v<DataType>) {
            std::cerr << "Data you want to pass to a message is not trivially copyable.";
            assert(false);
        }
        msg.extract_buf(reinterpret_cast<std::uint8_t *>(&dst), sizeof(DataType));
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
        if (std::is_trivially_copyable_v<ContainerType>) {
            *this << container;
        } else {
            for (auto &element : container) {
                *this << element;
            }
            *this << static_cast<std::uint32_t>(container.size());
        }
    }

    template <typename ContainerType>
    void extract_container(ContainerType &container) {
        if (std::is_trivially_copyable_v<ContainerType>) {
            *this >> container;
        } else {
            std::uint32_t container_size;
            *this >> container_size;
            container.resize(container_size);
            for (std::uint32_t i = container_size - 1; i + 1 != 0; --i) {
                *this >> container[i];
            }
        }
    }
};

template <typename T>
struct owned_message {
    std::shared_ptr<connection<T>> remote = nullptr;
    message<T> msg;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_MESSAGE_H
