class player {
public:
    std::string name{};
    std::string country{};
    int netid = -1;
    int userid = -1;
    int state = 32;
    vector2_t pos{};
    vector2_t RealPos() {
        return vector2_t(this->pos.m_x / 32, this->pos.m_y / 32);
    }
    bool invis{};
    bool mod{};
    player() {

    }
    bool operator==(const player& right) {
        return netid == right.netid && userid == right.userid;
    }
    player(std::string name, int netid, int uid) {
        this->name = name;
        this->netid = netid;
        this->userid = uid;
    }
};