#pragma once
#include <string>
namespace sticky_lotus::ui {
struct ProfileView {
    enum class Mode { Closed, Select, Scores, Keyboard, Winner, Undo };
    Mode mode=Mode::Closed;
    int page=0, seat=0, editing=-1;
    bool upper=true;
    std::string text, error;
};
}
