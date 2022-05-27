#include "../include/database_handler.h"
#include <random>
#include <stdexcept>

std::mt19937 gen;

database_handler &database_handler::get_instance() {
    static database_handler handler;
    return handler;
}

database_handler::database_handler() {
    gen.seed(time(nullptr));

    sqlite3 *database = open();

    create_database(database, std::string("CREATE TABLE IF NOT EXISTS PASSWORDS("
                                          "ID           INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "HANDLE       TEXT NOT NULL, "
                                          "HASH         TEXT NOT NULL, "
                                          "SALT         TEXT NOT NULL);"));
    create_database(database, std::string("CREATE TABLE IF NOT EXISTS RATINGS("
                                          "ID           INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "HANDLE       TEXT NOT NULL, "
                                          "RATING       INT  NOT NULL);"));
    create_database(database, std::string("CREATE TABLE IF NOT EXISTS STATS("
                                          "ID           INTEGER PRIMARY KEY AUTOINCREMENT, "
                                          "HANDLE_1     TEXT NOT NULL, "
                                          "RATING_1     INT  NOT NULL, "
                                          "HANDLE_2     TEXT NOT NULL, "
                                          "RATING_2     INT  NOT NULL,"
                                          "RESULT       INT  NOT NULL);"));

    sqlite3_close(database);
}

void database_handler::create_database(sqlite3 *database, const std::string &query) {
    char *messageError;
    int exit = sqlite3_exec(database, query.c_str(), nullptr, nullptr, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Failed to create/open table in database");
    }
}

sqlite3 *database_handler::open() {
    sqlite3 *database;
    int exit = sqlite3_open(database_name.c_str(), &database);
    if (exit != SQLITE_OK) {
        throw std::runtime_error("Failed to create/open database");
    }
    return database;
}

int database_handler::get_rating(const std::string &handle) {
    sqlite3 *database = open();

    int rating = -1;
    auto select = [](void *rating, int argc, char **argv, char **azColName) -> int {
        if (argc == 0)
            return 0;
        if (argc > 1)
            return 1;
        int *res = reinterpret_cast<int *>(rating);
        *res = std::stoi(argv[0]);
        return 0;
    };
    char *messageError;
    int exit =
        sqlite3_exec(database, ("SELECT RATING FROM RATINGS WHERE HANDLE = \'" + handle + '\'').c_str(),
                     select, &rating, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Error in get_rating");
    }

    sqlite3_close(database);

    return rating;
}

void database_handler::update_rating(const std::string &handle, Result res) {
    sqlite3 *database = open();
    char *messageError;
    int exit = sqlite3_exec(
        database,
        ("UPDATE RATINGS SET RATING = " + std::to_string(get_rating(handle) + (res == Result::WIN ? 1 : -1)) +
         " WHERE HANDLE = \'" + handle + "\'")
            .c_str(),
        nullptr, nullptr, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Error in update_rating function");
    }
    sqlite3_close(database);
}

void database_handler::add_game(database_handler::Result result,
                                const std::string &handle1,
                                const std::string &handle2) {
    sqlite3 *database = open();
    char *messageError;
    int exit =
        sqlite3_exec(database,
                     ("INSERT INTO STATS (HANDLE_1, RATING_1, HANDLE_2, RATING_2, RESULT) VALUES(\'" +
                      handle1 + "\', " + std::to_string(get_rating(handle1)) + ", \'" + handle2 + "\', " +
                      std::to_string(get_rating(handle2)) + ", " + (result == Result::WIN ? '1' : '0') + ");")
                         .c_str(),
                     nullptr, nullptr, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Error in add_game function");
    }
    sqlite3_close(database);

    update_rating(handle1, result);
    update_rating(handle2, static_cast<Result>(1 - static_cast<int>(result)));
}

std::vector<database_handler::Statistics_game> database_handler::get_statistics(const std::string &handle) {
    sqlite3 *database = open();

    std::pair<std::vector<database_handler::Statistics_game>, std::string> result;
    result.second = handle;
    auto select = [](void *stats, int argc, char **argv, char **azColName) -> int {
        if (argc == 0)
            return 0;
        if (argc != 5)
            return 1;
        auto *statistics =
            reinterpret_cast<std::pair<std::vector<database_handler::Statistics_game>, std::string> *>(stats);
        if (argv[0] == statistics->second) {
            statistics->first.emplace_back(argv[2], std::stoi(argv[1]), std::stoi(argv[3]),
                                           static_cast<Result>(std::stoi(argv[4])));
        } else {
            statistics->first.emplace_back(argv[0], std::stoi(argv[3]), std::stoi(argv[1]),
                                           static_cast<Result>(1 - std::stoi(argv[4])));
        }
        return 0;
    };
    char *messageError;
    int exit =
        sqlite3_exec(database,
                     ("SELECT HANDLE_1, RATING_1, HANDLE_2, RATING_2, RESULT FROM STATS WHERE HANDLE_1 = \'" +
                      handle + "\' OR HANDLE_2 = \'" + handle + "\'")
                         .c_str(),
                     select, &result, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Error in get_stats function");
    }

    sqlite3_close(database);

    return result.first;
}

bool database_handler::registration(const std::string &handle, const std::string &password) {
    sqlite3 *database = open();
    char *messageError;

    if (get_rating(handle) != -1) {
        return false;
    }

    int exit = sqlite3_exec(database,
                            ("INSERT INTO RATINGS (HANDLE, RATING) VALUES(\'" + handle + "\', " +
                             std::to_string(INITIAL_RATING) + ");")
                                .c_str(),
                            nullptr, nullptr, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Error in registration function");
    }

    std::string salt = generate_salt();
    exit = sqlite3_exec(database,
                        ("INSERT INTO PASSWORDS (HANDLE, HASH, SALT) VALUES(\'" + handle + "\', \'" +
                         std::to_string(hash_with_salt(password, salt)) + "\', \'" + salt + "\');")
                            .c_str(),
                        nullptr, nullptr, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Error in registration function");
    }
    sqlite3_close(database);

    return true;
}

bool database_handler::sign_in(const std::string &handle, const std::string &password) {
    sqlite3 *database = open();

    std::pair<bool, std::string> result;
    result.first = false;
    result.second = password;
    auto select = [](void *res, int argc, char **argv, char **azColName) -> int {
        if (argc == 0)
            return 0;
        if (argc != 2)
            return 1;
        auto *result = reinterpret_cast<std::pair<bool, std::string> *>(res);
        result->first = (argv[0] == std::to_string(hash_with_salt(result->second, argv[1])));
        return 0;
    };
    char *messageError;
    int exit =
        sqlite3_exec(database, ("SELECT HASH, SALT FROM PASSWORDS WHERE HANDLE = \'" + handle + "\'").c_str(),
                     select, &result, &messageError);
    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
        throw std::runtime_error("Error in sign_in function");
    }

    sqlite3_close(database);

    return result.first;
}

std::size_t database_handler::hash_with_salt(const std::string &password, const std::string &salt) {
    return std::hash<std::string>{}(salt + password);
}

std::string database_handler::generate_salt() const {
    std::string salt;
    for (int i = 0; i < SALT_LENGTH; ++i) {
        salt += static_cast<char>(gen() % 26 + 'a');
    }
    return salt;
}
