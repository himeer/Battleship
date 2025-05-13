#ifndef NTL__EVENT_HPP
#define NTL__EVENT_HPP

#include <variant>
#include "../Utils/Vector2.hpp"

namespace ntl {

class Event {
public:
    struct KeyPressed {
        int code{};
        int scancode{};
        int modes{};
    };

    struct KeyReleased {
        int code{};
        int scancode{};
        int modes{};
    };

    struct MouseMoved {
        Vector2i position;
    };

    struct MouseButtonPressed {
        Vector2i position;
        int button{};
        int modes{};
    };

    struct MouseButtonReleased {
        Vector2i position;
        int button{};
        int modes{};
    };


    template<class EventSubtype>
    Event(const EventSubtype &event) :
        data_(event)
    {}


    template<class EventSubtype>
    bool is() const {
        return std::holds_alternative<EventSubtype>(data_);
    }

    template<class EventSubtype>
    EventSubtype* getIf() {
        return std::get_if<EventSubtype>(&data_);
    }

    template<class EventSubtype>
    const EventSubtype* getIf() const {
        return std::get_if<EventSubtype>(&data_);
    }

private:
    std::variant<
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved
    > data_;
};

}

#endif // NTL__EVENT_HPP