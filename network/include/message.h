#ifndef WAR_OF_AGES_MESSAGE_H
#define WAR_OF_AGES_MESSAGE_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

namespace war_of_ages {

/// Works as stack: you need to extract elements in reversed order.
/// For non-trivially copyable types such as string use existing functions (i.e. insert_container()) or
/// implement ones.
/// Note that all sizes stored in message are represented by std::uint32_t.
template <typename T>
struct message {
private:
    struct message_header {
        T m_id{};
        std::uint32_t m_size = 0;
        explicit message_header(T id) : m_id(id){};
    } m_header;
    std::vector<std::uint8_t> m_data;

public:
    explicit message(T id) : m_header(id){};

    [[nodiscard]] std::size_t size() const noexcept {
        return sizeof(m_header) + m_data.size();
    }

    friend std::ostream &operator<<(std::ostream &out, const message &msg) {  // in case of debugging
        out << "Message (id=" << msg.header.id << "): size=" << msg.size() << "\n";
        return out;
    }

    template <typename DataType>
    friend message &operator<<(message &msg, const DataType &src) {
        static_assert(std::is_trivially_copyable_v<DataType>,
                      "Data you want to pass to a message is not trivially copyable.");
        std::uint32_t insertion_pos = msg.m_data.size();
        msg.m_data.resize(msg.m_data.size() + sizeof(src));
        memcpy(msg.m_data.data() + insertion_pos, reinterpret_cast<const std::uint8_t *>(&src), sizeof(src));
        msg.m_header.m_size = msg.size();
        return msg;
    }

    template <typename DataType>
    friend message &operator>>(message &msg, DataType &dst) {
        static_assert(std::is_trivially_copyable_v<DataType>,
                      "Data you want to extract from a message is not trivially copyable.");
        assert(msg.m_data.size() >= sizeof(dst));
        std::uint32_t extraction_pos = msg.m_data.size() - sizeof(dst);
        memcpy(reinterpret_cast<std::uint8_t *>(&dst), msg.m_data.data() + extraction_pos, sizeof(dst));
        msg.m_data.resize(extraction_pos);
        msg.m_header.m_size = msg.size();
        return msg;
    }

    void insert_buf(std::uint8_t *buf, std::uint32_t number_of_bytes) {
        std::uint32_t insertion_pos = m_data.size();
        m_data.resize(m_data.size() + number_of_bytes);
        memcpy(m_data.data() + insertion_pos, buf, number_of_bytes);
        m_header.m_size = size();
    }

    void extract_buf(std::uint8_t *buf, std::uint32_t number_of_bytes) {
        assert(m_data.size() >= number_of_bytes);
        std::uint32_t extraction_pos = m_data.size() - number_of_bytes;
        memcpy(buf, m_data.data() + extraction_pos, number_of_bytes);
        m_data.resize(extraction_pos);
        m_header.m_size = size();
    }

    template <typename ContainerType>
    void insert_container(const ContainerType &container) {
        static_assert(std::is_trivially_copyable_v<typename ContainerType::value_type>,
                      "Data stored in container is not trivially copyable.");
        std::uint32_t value_size = sizeof(typename ContainerType::value_type);
        std::uint32_t insertion_pos = m_data.size();
        std::uint32_t container_size = container.size();
        m_data.resize(m_data.size() + container_size * value_size + sizeof(container_size));
        memcpy(m_data.data() + insertion_pos, reinterpret_cast<const std::uint8_t *>(container.data()),
               container_size * value_size);  // TODO: check if it is ok
        memcpy(m_data.data() + insertion_pos + value_size * container_size,
               reinterpret_cast<std::uint8_t *>(&container_size), sizeof(container_size));
        m_header.m_size = size();
    }

    template <typename ContainerType>
    void extract_container(ContainerType &container) {
        static_assert(std::is_trivially_copyable_v<typename ContainerType::value_type>,
                      "Data stored in container is not trivially copyable.");
        std::uint32_t container_size;
        *this >> container_size;
        std::uint32_t value_size = sizeof(typename ContainerType::value_type);
        std::uint32_t extraction_pos = m_data.size() - container_size * value_size;
        container.assign(container_size, typename ContainerType::value_type{});
        memcpy(reinterpret_cast<std::uint8_t *>(container.data()), m_data.data() + extraction_pos,
               container_size * value_size);  // TODO: check if it is ok
        m_data.resize(extraction_pos);
        m_header.m_size = size();
    }
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_MESSAGE_H
