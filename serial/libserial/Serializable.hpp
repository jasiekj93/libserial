#pragma once

/**
 * @file Serializable.hpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <optional>

namespace serial
{
    class Serializer;

    class Serializable
    {
    public:
        virtual void serialize(Serializer&) const = 0;
    };
}