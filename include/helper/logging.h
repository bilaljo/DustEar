#pragma once

/* A simple, but feature-complete header-only logging library for Windows and POSIX
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the “Software”), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software. THE SOFTWARE IS PROVIDED “AS IS”,
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <format>
#include <iostream>
#include <sstream>
#include <functional>
#include <chrono>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif


class logging {
private:
    static inline std::vector<std::function<void(const std::string&)>> observers;

    struct Colour {
        static constexpr std::string_view green = "\\033[92m";

        static constexpr std::string_view blue = "\\033[34m";

        static constexpr std::string_view yellow = "\\033[33m";

        static constexpr std::string_view red = "\\033[91m";

        static constexpr std::string_view purple = "\\033[35m";

        static constexpr std::string_view reset = "\\033[0m";
    };

public:
    enum class LogLevel {
        debug,
        info,
        warning,
        error,
        criticial
    };


    static inline auto log_level = LogLevel::debug;

private:
#ifdef WIN32
    static void set_console_color(const std::string_view colour) noexcept {
        const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (colour == Colour::yellow) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        } else if (colour == Colour::blue) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        } else if (colour == Colour::red) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        } else if (colour == Colour::green) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        } else if (colour == Colour::purple) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
        } else if (colour == Colour::reset) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    }
#endif

public:
    static void add_handler(const std::function<void(const std::string&)>& observer) noexcept {
        observers.push_back(observer);
    }

private:
    static void inform_handler(const std::string& message) {
        for (const auto& observer: observers) {
            observer(message);
        }
    }

    template<typename... Args>
    static void print_message(std::string_view message, LogLevel max_level,
                       std::string_view colour, std::string_view log_type,
                       std::format_args &&args) {
        if (log_level <= max_level) {
            const auto now = std::chrono::system_clock::now();
            auto local_time = std::chrono::current_zone()->to_local(now);
#ifdef _WIN32
            set_console_color(colour);
#else
            std::cout << colour;
#endif
            std::stringstream output;
            output << log_type << " " << std::format("{:%Y-%m-%d %H:%M:%OS}", local_time) << ": ";
            output << std::vformat(message, args) << "\n";
            inform_handler(output.str());
            if (log_level >= LogLevel::warning) {
                std::cerr << output.str();
            } else {
                std::cout << output.str();
            }
        }
#ifdef _WIN32
        set_console_color(Colour::reset);
#else
        std::cout << Colour::reset;
#endif
    }

public:
    template<typename... Args>
    static void debug(const std::string_view message, Args&&... args) {
        print_message(message, LogLevel::debug, Colour::blue, "DEBUG",
                      std::make_format_args(args...));
    }

    template<typename... Args>
    static void info(const std::string_view message, Args&&... args) {
        print_message(message, LogLevel::info, Colour::green, "INFO",
                      std::make_format_args(args...));
    }

    template<typename... Args>
    static void warning(const std::string_view message, Args&&... args) {
        print_message(message, LogLevel::warning, Colour::yellow, "WARNING",
                      std::make_format_args(args...));
    }

    template<typename... Args>
    static void error(const std::string_view message, Args&&... args) {
        print_message(message, LogLevel::error, Colour::red, "ERROR",
                      std::make_format_args(args...));
    }

    template<typename... Args>
    static void criticial(const std::string_view message, Args&&... args) {
        print_message(message, LogLevel::criticial, Colour::purple, "CRITICIAL",
                      std::make_format_args(args...));
    }
};
