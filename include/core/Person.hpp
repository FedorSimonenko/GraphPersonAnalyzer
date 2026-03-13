#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <vector>
#include <utility>

namespace graph_analyzer {

    struct Person {
        std::string name;
        std::string surname;
        size_t age = 0;
        bool isMale = true;
        bool isResidense = true;
        size_t weight = 0;
        size_t height = 0;
        std::vector<std::pair<short, short>> routes;

        Person() = default;
        Person(std::string name, std::string surname, size_t age, bool isMale,
               bool isResidense, size_t weight, size_t height,
               std::vector<std::pair<short, short>> routes);

        ~Person() = default;
        Person(const Person&) = default;
        Person& operator=(const Person&) = default;
        Person(Person&&) = default;
        Person& operator=(Person&&) = default;

        double getAge() const;
        double getWeight() const;
        double getHeight() const;
        double getMale() const;
        double getResidense() const;

        static constexpr size_t getMinAge() {return 3;}
        static constexpr size_t getMaxAge() {return 120;}
        static constexpr size_t getMinWeight() {return 12;}
        static constexpr size_t getMaxWeight() {return 240;}
        static constexpr size_t getMinHeight() {return 80;}
        static constexpr size_t getMaxHeight() {return 270;}
    };

}

#endif