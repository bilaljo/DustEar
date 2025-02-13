#pragma once

/* A simple but powerfull event bus library.
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

#include <vector>
#include <functional>
#include <string>
#include <any>
#include <deque>
#include <mutex>

namespace helper {
    class EventBus {
    public:
        void register_topic(const std::string& topic) {
            topics.push_back(topic);
        }

        void register_signal_qualifier(const std::string& qualifier) {
            signal_qualifiers.push_back(qualifier);
        }

        void register_subscriber(const std::function<void(std::any)>& subscriber, const std::string& topic) {
            subscribers.emplace_back(topic, subscriber);
            if (!unfired_publishes.empty()) {
                for (const auto& [saved_topic, saved_message]: unfired_publishes) {
                    publish(saved_topic, saved_message);
                    unfired_publishes.pop_front();
                }
            }
        }

        void register_signal(const std::function<void()>& signal, const std::string& qualifier) {
            signals.emplace_back(qualifier, signal);
            if (!unfired_signals.empty()) {
                for (const auto& save_signal_qualifier: unfired_signals) {
                    emit(save_signal_qualifier);
                    unfired_signals.pop_front();
                }
            }
        }

        template<typename T>
        void publish(const std::string& topic, T message) {
            bool fired = false;
            for (const auto& [registered_topic, subscriber]: subscribers) {
                if (topic == registered_topic) {
                    subscriber(message);
                    fired = true;
                }
            }
            if (!fired) {
                // Topic is registered but no subscriber was available hence
                // we have to fire it as soon one is avaiable
                unfired_publishes.push_front({topic, message});
            }
        }

        void emit(const std::string& signal_qualifier) {
            bool fired = false;
            for (const auto& [registered_signal_qualifier, signal]: signals) {
                if (signal_qualifier == registered_signal_qualifier) {
                    signal();
                    fired = true;
                }
            }
            if (!fired) {
                // Topic is registered but no subscriber was available hence
                // we have to fire it as soon one is avaiable
                unfired_signals.push_front(signal_qualifier);
            }
        }

    private:
        std::vector<std::string> topics;

        std::vector<std::string> signal_qualifiers;

        std::vector<std::pair<std::string, std::function<void(std::any)>>> subscribers;

        std::vector<std::pair<std::string, std::function<void()>>> signals;

        std::vector<std::pair<std::string, std::any>> messages;

        std::deque<std::pair<std::string, std::any>> unfired_publishes;

        std::deque<std::string> unfired_signals;
    };
}
