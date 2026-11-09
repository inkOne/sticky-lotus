#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <limits>

namespace sticky_lotus {
struct PlayerProfile { char name[17]{}; std::uint32_t wins=0; };
struct ProfileSnapshot {
    std::uint32_t magic=0x50524631;
    std::uint32_t count=0;
    std::array<PlayerProfile,32> players{};
    std::array<std::int32_t,4> seats{{-1,-1,-1,-1}};
    std::int32_t lastWinner=-1;
};
class PlayerProfiles {
    ProfileSnapshot data_{};
    bool dirty_=false;
    static char lower(char c) { return c>='A' && c<='Z' ? c+('a'-'A') : c; }
    static bool same(std::string_view a,std::string_view b) {
        if(a.size()!=b.size()) return false;
        for(std::size_t i=0;i<a.size();++i) if(lower(a[i])!=lower(b[i])) return false;
        return true;
    }
public:
    // Starts empty: names are added in the player dialog ("New player").
    PlayerProfiles() = default;
    const ProfileSnapshot& snapshot() const { return data_; }
    bool dirty() const { return dirty_; }
    void saved() { dirty_=false; }
    std::size_t size() const { return data_.count; }
    const PlayerProfile& at(std::size_t i) const { return data_.players[i]; }
    int seat(std::size_t i) const { return i<4 ? data_.seats[i] : -1; }
    std::string seatName(std::size_t i) const {
        return seat(i)<0 ? "Player "+std::to_string(i+1) : at(seat(i)).name;
    }
    bool used(int id,std::size_t except,std::size_t active) const {
        for(std::size_t i=0;i<active && i<4;++i) if(i!=except && seat(i)==id && id>=0) return true;
        return false;
    }
    bool assign(std::size_t seatIndex,int id,std::size_t active) {
        if(seatIndex>=active || active>4 || id<-1 || id>=static_cast<int>(size())) return false;
        // Move a selected profile from any other seat; the old seat becomes a guest.
        if(id>=0) for(std::size_t i=0;i<4;++i) if(i!=seatIndex && data_.seats[i]==id) data_.seats[i]=-1;
        data_.seats[seatIndex]=id; dirty_=true; return true;
    }
    bool validName(std::string_view name,int except=-1) const {
        if(name.empty() || name.size()>16 || name.front()==' ' || name.back()==' ') return false;
        for(char c:name) if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c==' '||c=='-'||c=='\'')) return false;
        for(std::size_t i=0;i<size();++i) if(static_cast<int>(i)!=except && same(name,at(i).name)) return false;
        return true;
    }
    int add(std::string_view name) {
        if(size()>=data_.players.size() || !validName(name)) return -1;
        const int id=data_.count++;
        std::memcpy(data_.players[id].name,name.data(),name.size());
        dirty_=true; return id;
    }
    bool rename(int id,std::string_view name) {
        if(id<0 || id>=static_cast<int>(size()) || !validName(name,id)) return false;
        auto& target=data_.players[id]; std::memset(target.name,0,sizeof(target.name));
        std::memcpy(target.name,name.data(),name.size()); dirty_=true; return true;
    }
    bool win(std::size_t seatIndex) {
        const int id=seat(seatIndex);
        if(id<0) return false;
        auto& wins=data_.players[id].wins;
        if(wins==std::numeric_limits<std::uint32_t>::max()) return false;
        ++wins; data_.lastWinner=id; dirty_=true; return true;
    }
    bool canUndo() const { return data_.lastWinner>=0; }
    bool undo() {
        if(!canUndo()) return false;
        --data_.players[data_.lastWinner].wins; data_.lastWinner=-1; dirty_=true; return true;
    }
    void changed() { dirty_=true; }
    bool restore(const ProfileSnapshot& s) {
        if(s.magic!=0x50524631 || s.count>32 || s.lastWinner<-1 || s.lastWinner>=static_cast<int>(s.count)) return false;
        for(std::size_t i=0;i<s.count;++i) {
            const auto* end=static_cast<const char*>(std::memchr(s.players[i].name,0,17));
            if(!end || end==s.players[i].name) return false;
            std::string_view name(s.players[i].name,end-s.players[i].name);
            if(name.front()==' ' || name.back()==' ') return false;
            for(char c:name) if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c==' '||c=='-'||c=='\'')) return false;
            for(std::size_t j=0;j<i;++j) if(same(name,s.players[j].name)) return false;
        }
        for(std::size_t i=0;i<4;++i) {
            if(s.seats[i]<-1 || s.seats[i]>=static_cast<int>(s.count)) return false;
            for(std::size_t j=0;j<i;++j) if(s.seats[i]>=0 && s.seats[i]==s.seats[j]) return false;
        }
        if(s.lastWinner>=0 && s.players[s.lastWinner].wins==0) return false;
        data_=s; dirty_=false; return true;
    }
};
}
