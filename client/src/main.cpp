#include "../include/application.h"

int main() {
    war_of_ages::client::application::init();
    war_of_ages::client::application::instance().run();
}