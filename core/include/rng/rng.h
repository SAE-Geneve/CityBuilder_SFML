//
// Created by sebas on 10.06.2026.
//

#ifndef CITYBUILDER_RNG_H
#define CITYBUILDER_RNG_H
#include <random>
#include <SFML/System/Vector2.hpp>

namespace core::rng {
    template<typename T>
    sf::Vector2<T> get_vector(sf::Vector2<T> bounds){

        sf::Vector2<T> result;
        result.x = get_value<T>(0,bounds.x);
        result.y = get_value<T>(0,bounds.y);
        return result;
    }

    template<typename T>
    T get_value(T min, T max){
        std::uniform_real_distribution<T> dist(min, max);
        std::mt19937 rng_{std::random_device{}()};
        return dist(rng_);
    }
}

#endif //CITYBUILDER_RNG_H
