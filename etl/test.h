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

 #if !defined(ETL_TEST_H)
#define ETL_TEST_H

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

namespace etl {

class Test
{
private:
    enum class Color
    {
        Red = 31,
        Green = 32,
        Yellow = 33,
        Blue = 34,
        Magenta = 35,
        Cyan = 36,
        White = 37
    };

    template<Color C>
    static std::string bash(std::string text)
    {
        return "\033[" + std::to_string(static_cast<int>(C)) + "m" + text + "\033[0m";
    }

    static std::string name(std::string test)
    {
        return "Test( " + bash<Color::Cyan>(test) + " )";
    }

    static std::string pass()
    {
        return " - [ " + bash<Color::Green>("PASSED") + " ]";
    }

    static std::string fail()
    {
        return " - [ " + bash<Color::Red>("FAILED") + " ]";
    }

    bool success;
    const std::string test;

public:
    explicit Test(const std::string& test) : success(true), test(test)
    {
    }

    ~Test()
    {
        std::cout << name(test);
        std::cout << (success ? pass() : fail()) << std::endl;
        if(!success)
        {
            exit(1);
        }
    }

    void expect(bool condition, const char* file, int line)
    {
        if (!condition)
        {
            success = false;
            std::cerr << "Expectation failed @ " << std::string(file) << ":" << std::to_string(line) << std::endl;
        }
    }

    template<typename Container>
    static std::string print(const Container& container)
    {
        std::ostringstream oss;
        oss << "{ ";
        for (const auto& item : container)
        {
            oss << "0x"
                << std::setw(sizeof(item) * 2)
                << std::setfill('0')
                << std::hex
                << std::nouppercase
                << static_cast<uint64_t>(item)
                << ",";
        }
        oss.seekp(-1, std::ios_base::end);
        oss << " }";
        
        return oss.str();
    }
};

#define TEST(name) etl::Test test(name)
#define EXPECT(condition) test.expect((condition), __FILE__, __LINE__)

} // namespace etl

#endif