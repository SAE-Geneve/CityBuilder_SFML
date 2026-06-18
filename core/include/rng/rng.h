//
// Created by sebas on 10.06.2026.
//

#ifndef CITYBUILDER_RNG_H
#define CITYBUILDER_RNG_H
#include <random>
#include <SFML/System/Vector2.hpp>

namespace core::rng {

    inline std::uniform_real_distribution<double> dist_float(0.f, 1.f);
    inline std::mt19937 rng_{std::random_device{}()};

    template<typename T>
    requires std::is_floating_point_v<T>
    T get_value(T min, T max){
        return min + max * dist_float(rng_);
    }

    template<typename T>
    requires std::is_integral_v<T>
    T get_value(T min, T max){
        return min + max * dist_float(rng_);
    }

    template<typename T>
    sf::Vector2<T> get_vector(sf::Vector2<T> bounds){

        sf::Vector2<T> result;
        result.x = get_value<T>(0,bounds.x);
        result.y = get_value<T>(0,bounds.y);
        return result;
    }

}

#endif //CITYBUILDER_RNG_H
