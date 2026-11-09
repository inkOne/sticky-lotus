#include "sticky_lotus/app/Application.h"
namespace sticky_lotus::app {
void Application::openProfiles(ui::ProfileView::Mode mode) {
    screenManager_.cancelPingUndo();
    diceView_=DiceView::Closed;
    profileView_.mode=mode; profileView_.page=0; profileView_.error.clear();
    draw();
}
void Application::handleProfileInput(const input::InputFrame& input) {
    using Mode=ui::ProfileView::Mode;
    auto& v=profileView_;
    auto back=[&]() {
        if(v.mode==Mode::Keyboard) { v.mode=keyboardReturn_; v.error.clear(); }
        else if(v.mode==Mode::Undo) v.mode=Mode::Scores;
        else v.mode=Mode::Closed;
        draw();
    };
    if(input.cancelPressed || input.buttons.centerPressed) {back(); return;}
    if(input.gesture.gesture!=input::TouchGesture::Tap && input.gesture.gesture!=input::TouchGesture::LongPress) return;
    const auto p=input.gesture.endPosition;
    auto footer=[&](int i){return ui::AppRenderer::profileFooter(i).contains(p);};
    auto nav=[&](int i){return ui::AppRenderer::profileNav(i).contains(p);};
    auto finish=[&](int seat) {
        if(seat>=0) game_.profiles.win(seat);
        game_.profiles.changed(); // Persist reset and result in the same session blob.
        resetGame();
    };
    if(v.mode==Mode::Winner) {
        if(footer(2)) {back();return;}
        if(footer(0)) {finish(-1);return;}
        for(std::size_t i=0;i<game_.getPlayerCount();++i)
            if(ui::AppRenderer::profileRow(i).contains(p)) {finish(i); return;}
        return;
    }
    if(v.mode==Mode::Undo) {
        if(footer(0)) {game_.profiles.undo();v.mode=Mode::Scores;draw();}
        else if(footer(2)) back();
        return;
    }
    if(v.mode==Mode::Keyboard) {
        if(footer(2)) {back();return;}
        if(footer(0)) {
            // Spaces entered at the edges are harmless; normalise before validation.
            const auto first=v.text.find_first_not_of(' ');
            const auto last=v.text.find_last_not_of(' ');
            const std::string name=first==std::string::npos ? "" : v.text.substr(first,last-first+1);
            int id=v.editing;
            const bool success=id<0 ? ((id=game_.profiles.add(name))>=0) : game_.profiles.rename(id,name);
            if(!success) {v.error="Use a unique name, 1-16 characters.";draw();return;}
            if(keyboardReturn_==Mode::Select) {
                game_.profiles.assign(v.seat,id,game_.getPlayerCount());v.mode=Mode::Closed;
            } else {v.mode=Mode::Scores;v.page=id/5;}
            v.error.clear();draw();return;
        }
        if(nav(0)) v.upper=!v.upper;
        else if(nav(1)) {if(v.text.size()<16) v.text+=' ';}
        else if(nav(2)) {if(!v.text.empty()) v.text.pop_back();}
        else {
            const char* rows[]={"QWERTYUIOP","ASDFGHJKL-","ZXCVBNM'  ","0123456789"};
            for(int row=0;row<4;++row) for(int col=0;col<10;++col)
                if(ui::AppRenderer::keyboardKey(row,col).contains(p) && v.text.size()<16) {
                    char c=rows[row][col];if(c==' ') continue; if(!v.upper && c>='A' && c<='Z') c+=32;
                    v.text+=c;v.upper=false;
                }
        }
        v.error.clear();draw();return;
    }
    if(footer(2)) {back();return;}
    if(footer(0) && game_.profiles.size()<32) {
        keyboardReturn_=v.mode;v.mode=Mode::Keyboard;v.editing=-1;v.text.clear();v.error.clear();v.upper=true;draw();return;
    }
    if(footer(1)) {
        if(v.mode==Mode::Select) {
            game_.profiles.assign(v.seat,-1,game_.getPlayerCount());v.mode=Mode::Closed;
        } else if(game_.profiles.canUndo()) v.mode=Mode::Undo;
        draw();return;
    }
    if(nav(0) && v.page>0) --v.page;
    else if(nav(2) && (v.page+1)*5<static_cast<int>(game_.profiles.size())) ++v.page;
    else for(int row=0;row<5;++row) {
        const int id=v.page*5+row;
        if(id>=static_cast<int>(game_.profiles.size()) || !ui::AppRenderer::profileRow(row).contains(p)) continue;
        if(v.mode==Mode::Select) {
            if(game_.profiles.assign(v.seat,id,game_.getPlayerCount())) v.mode=Mode::Closed;
            else v.error="Could not select that player.";
        } else {
            keyboardReturn_=Mode::Scores;v.mode=Mode::Keyboard;v.editing=id;v.text=game_.profiles.at(id).name;v.error.clear();v.upper=true;
        }
        break;
    }
    draw();
}
}
