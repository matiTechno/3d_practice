#ifndef KEY_INPUT_HPP
#define KEY_INPUT_HPP

#include <set>

class Key_input
{
public:
    void start_new_frame();

    void press_event(int code);
    void release_event(int code);

    bool was_pressed(int code) const;
    bool is_pressed(int code) const;
    bool was_released(int code) const;

private:
    std::set<int> set_was_pressed;
    std::set<int> set_is_pressed;
    std::set<int> set_was_released;
};

inline void Key_input::start_new_frame()
{
    set_was_pressed.clear();
    set_was_released.clear();
}

inline void Key_input::press_event(int code)
{
    set_is_pressed.emplace(code);
    set_was_pressed.emplace(code);
}

inline void Key_input::release_event(int code)
{
    set_is_pressed.erase(code);
    set_was_released.emplace(code);
}

inline bool Key_input::was_pressed(int code) const
{
    auto it = set_was_pressed.find(code);
    if(it == set_was_pressed.end())
        return false;
    return true;
}

inline bool Key_input::is_pressed(int code) const
{
    auto it = set_is_pressed.find(code);
    if(it == set_is_pressed.end())
        return false;
    return true;
}

inline bool Key_input::was_released(int code) const
{
    auto it = set_was_released.find(code);
    if(it == set_was_released.end())
        return false;
    return true;
}

#endif // KEY_INPUT_HPP
