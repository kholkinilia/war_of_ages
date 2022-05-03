#include <boost/asio.hpp>
#include <iostream>
#include "../../network/include/network.h"

int main() {
    // sample of using message (remove if needed)

    enum class msg_type { BUY_UNIT, ADD_TO_TOURNAMENT };
    war_of_ages::message<msg_type> msg(msg_type::ADD_TO_TOURNAMENT);

    int val = 10;
    std::string handle = "fixmetodo";
    std::vector<int> a{1, 2, 3, 4, 5};
    struct {
        int x = 7;
        float a = 11;
    } b[3];

    msg.insert_container(handle);
    msg.insert_container(a);
    msg << val << b;

    a = {};
    handle = "";
    val = -1;

    msg >> b >> val;
    msg.extract_container(a);
    msg.extract_container(handle);

    std::cout << val << "\n" << handle << "\n";
    for (int i : a) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
