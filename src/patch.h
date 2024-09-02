#ifndef PATCH_H
#define PATCH_H

#include <unordered_map>

#include <glm/glm.hpp>

//using boost::hash_combine
template <class T>
inline void hash_combine(std::size_t& seed, T const& v)
{
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


// Adapted from https://stackoverflow.com/questions/37007307/fast-hash-function-for-stdvector
namespace std
{
    template<>
    struct hash<glm::vec<3, int>>
    {
        typedef std::size_t result_type;
        result_type operator()(glm::vec3 const& in) const
        {
            size_t seed = 0;
			hash_combine(seed, in.x);
			hash_combine(seed, in.y);
			hash_combine(seed, in.z);
            return seed;
        }
    };
}

#endif