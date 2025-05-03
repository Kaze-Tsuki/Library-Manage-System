#include "json.hpp"
#include <fstream>
#include "Library.h"

using json = nlohmann::json;

// ------------------ Book serialization ------------------
void to_json(json& j, const Book& b) {
    j = json{
        {"name", b.name},
        {"author", b.author},
        {"published", {b.published.year, b.published.month, b.published.day}},
        {"due", {b.due.year, b.due.month, b.due.day}},
        {"ISBN", b.ISBN},
        {"category", b.category},
        {"copyAmount", b.copyAmount},
        {"availableCopies", b.availableCopies}
    };
}

void from_json(const json& j, Book& b) {
    auto pub = j.at("published");
    auto due = j.at("due");
    b.name = j.at("name").get<std::string>();
    b.author = j.at("author").get<std::string>();
    b.published = Date(pub[0], pub[1], pub[2]);
    b.due = Date(due[0], due[1], due[2]);
    b.ISBN = j.at("ISBN").get<std::string>();
    b.category = j.at("category").get<std::string>();
    b.copyAmount = j.at("copyAmount").get<size_t>();
    b.availableCopies = j.at("availableCopies").get<size_t>();
}

// ------------------ User serialization ------------------
void to_json(json& j, const User& u) {
    j = json{
        {"name", u.name},
        {"borrowing", u.borrowing}
    };
}

void from_json(const json& j, User& u) {
    u.name = j.at("name").get<std::string>();
    if (j.contains("borrowing")) {
        u.borrowing = j.at("borrowing").get<std::vector<Book>>();
    }
    else {
        u.borrowing = {};  // 空的 borrowing
    }
}

// ------------------ Library save/load ------------------


// 反序列化 std::vector<User>
void from_json(const nlohmann::json& j, std::vector<User>& users) {
    users = j.get<std::vector<User>>();  // 使用該類型的 from_json
}

void saveLibrary(Library& lib, const std::string& filename) {
    json j;
    auto books = lib.getBooks();
    auto users = lib.getUsers();

    std::vector<Book> bookValues;
    for (auto b : books) {
        if (b) bookValues.push_back(*b);
    }

    j["books"] = bookValues;
    j["users"] = users;

    std::ofstream out(filename);
    out << j.dump(4);
}

void loadLibrary(Library& lib, const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return;
    json j;
    in >> j;

    std::vector<Book> bookValues = j["books"].get<std::vector<Book>>();
    for (auto& b : bookValues) {
        Book* ptr = new Book(b);
        lib.addBook(ptr);
    }

    for (const auto& item : j["users"]) {
        User u;
        u.name = item["name"].get<std::string>();

        for (const auto& bookJson : item["borrowing"]) {
            Book b;

            auto pub = bookJson["published"];
            auto due = bookJson["due"];

            b.name = bookJson["name"].get<std::string>();
            b.author = bookJson["author"].get<std::string>();
            b.published = Date(pub[0], pub[1], pub[2]);
            b.due = Date(due[0], due[1], due[2]);
            b.ISBN = bookJson["ISBN"].get<std::string>();
            b.category = bookJson["category"].get<std::string>();
            b.copyAmount = bookJson["copyAmount"].get<size_t>();
            b.availableCopies = bookJson["availableCopies"].get<size_t>();

            u.borrowing.push_back(b);
        }

        lib.addUser(u);
    }
}
