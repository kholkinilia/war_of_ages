#include "../include/application.h"
#include "../include/client.h"

namespace war_of_ages {

war_of_ages::client_state current_state;

}  // namespace war_of_ages

int main() {
    war_of_ages::application::init();
    war_of_ages::application::instance().run();
}