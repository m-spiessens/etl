/* The MIT License (MIT)
 *
 * Copyright (c) 2025 Mathias Spiessens
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software, hardware and associated documentation files (the "Solution"), to deal
 * in the Solution without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Solution, and to permit persons to whom the Solution is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Solution.
 *
 * THE SOLUTION IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOLUTION OR THE USE OR OTHER DEALINGS IN THE
 * SOLUTION.
 */

#if !defined(ETL_MAP_H_)
#define ETL_MAP_H_

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <array>
#include <initializer_list>
#include <utility>
namespace etl
{

template<typename Key, typename Value, uint16_t N>
class Map
{
public:
    /**
     * \brief Create a map with an initializer list of entries.
     */
    constexpr Map(std::initializer_list<std::pair<Key, Value>> list) : data{} {
        std::size_t i = 0;
        for (auto& p : list) {
            if (i < N) {
                data[i++] = p;
            }
            if(i > 1)
            {
                // Ensure the data is sorted by key
                for (std::size_t j = i - 1; j > 0 && data[j].first < data[j - 1].first; --j) {
                    std::swap(data[j], data[j - 1]);
                }
            }
        }
    }

    constexpr void exist(const Key& key) const
    {
        uint16_t left = 0;
        uint16_t right = N;
        while (left < right) {
            uint16_t mid = left + (right - left) / 2;
            if (data[mid].first == key) {
                return;
            } else if (data[mid].first < key) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        assert(false);
    }

    constexpr const Value& operator[](const Key& key) const
    {
        uint16_t left = 0;
        uint16_t right = N;
        while (left < right) {
            uint16_t mid = left + (right - left) / 2;
            if (data[mid].first == key) {
                return data[mid].second;
            } else if (data[mid].first < key) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        assert(false);
        return dummy;
    }

    Value& at(const Key& key)
    {
        uint16_t left = 0;
        uint16_t right = N;
        while (left < right) {
            uint16_t mid = left + (right - left) / 2;
            if (data[mid].first == key) {
                return data[mid].second;
            } else if (data[mid].first < key) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        assert(false);
        return dummy;
    }

    constexpr uint16_t size() const { return N; }

    constexpr std::array<Key, N> keys() const
    {
        std::array<Key, N> _keys{};
        for (uint16_t i = 0; i < N; ++i) {
            _keys[i] = data[i].first;
        }
        return _keys;
    }

    constexpr std::array<Value, N> values() const
    {
        std::array<Value, N> _values{};
        for (uint16_t i = 0; i < N; ++i) {
            _values[i] = data[i].second;
        }
        return _values;
    }

private:
    Value dummy{};
    std::array<std::pair<Key, Value>, N> data;
};

} // namespace etl

#endif