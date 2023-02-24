#ifndef GrowtopiaHelper_CLASS
#define GrowtopiaHelper_CLASS
#ifdef _WIN32
#pragma warning(disable : 4307)

#define INLINE __forceinline
#else //for gcc/clang

#define INLINE inline
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <fstream> 
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include "../../library/enet/include/enet.h"
#include <random>

#define HashCoord(x, y) (((y) * 100) + (x))
#define TILE_SIZE 32
#define C_VAR_SPACE_BYTES 16
template <typename t, t val>
class cexpr {
public:
    static constexpr t value = val;
};
namespace hs {
    constexpr uint32_t val_32_const = 0x811c9dc5u;
    constexpr uint32_t prime_32_const = 0x1000193u;
    constexpr uint64_t val_64_const = 0xcbf29ce484222325u;
    constexpr uint64_t prime_64_const = 0x100000001b3u;
    INLINE constexpr uint32_t hash32(const char* const str, const uint32_t value = val_32_const) noexcept {
        return (str[0] == '\0') ? value : hash32(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
    }
    INLINE constexpr uint64_t hash64(const char* const str, const uint64_t value = val_64_const) noexcept {
        return (str[0] == '\0') ? value : hash64(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
    }
} // namespace hs
#define fnv32(s) (cexpr<uint32_t, hs::hash32(s)>::value)
#define fnv64(s) (cexpr<uint64_t, hs::hash64(s)>::value)
#define get_packet_type(packet) (packet->dataLength > 3 ? *packet->data : 0)
#pragma pack(push, 1)
struct GameUpdatePacket {
    uint8_t type{}; //0
    union {
        uint8_t objtype{}; //1
        uint8_t punchid;
        uint8_t npctype;
    };
    union {
        uint8_t count1{}; //2
        uint8_t jump_count;
        uint8_t build_range;
        uint8_t npc_id;
        uint8_t lost_item_count;
    };
    union {
        uint8_t count2{}; //3
        uint8_t animation_type;
        uint8_t punch_range;
        uint8_t npc_action;
        uint8_t particle_id;
        uint8_t gained_item_count;
        uint8_t dice_result;
        uint8_t fruit_count;
    };
    union {
        int32_t netid{}; //4
        int32_t effect_flags_check;
        int32_t object_change_type;
        int32_t particle_emitter_id;
    };
    union {
        int32_t item{}; //8
        int32_t ping_hash;
        int32_t item_netid;
        int32_t pupil_color;
        int32_t tiles_length;
    };
    int32_t flags{}; //12 - 0xC
    union {
        float float_var{}; //16 - 0x10
        float water_speed;
        float obj_alt_count;
    };
    union {
        int32_t int_data{}; //20 - 0x14
        int32_t ping_item;
        int32_t elapsed_ms;
        int32_t delay;
        int32_t tile_damage;
        int32_t item_id;
        int32_t item_speed;
        int32_t effect_flags;
        int32_t object_id;
        int32_t hash;
        int32_t verify_pos;
        int32_t client_hack_type;
    };
    union {
        float vec_x{}; //24 - 0x18
        float pos_x;
        float accel;
        float punch_range_in;
    };
    union {
        float vec_y{}; //28 - 0x1C
        float pos_y;
        float build_range_in;
        float punch_strength;
    };
    union {
        float vec2_x{}; //32 - 0x20
        float dest_x;
        float gravity_in;
        float speed_out;
        float velocity_x;
        float particle_variable;
        float pos2_x;
        int hack_type;
    };
    union {
        float vec2_y{}; //36 - 0x24
        float dest_y;
        float speed_in;
        float gravity_out;
        float velocity_y;
        float particle_alt_id;
        float pos2_y;
        int hack_type2;
    };
    union {
        float particle_rotation{}; //40 - 0x28
        float npc_variable;
    };
    union {
        uint32_t int_x{}; //44 - 0x2C
        uint32_t item_id_alt;
        uint32_t eye_shade_color;
    };
    union {
        uint32_t int_y{}; //48 - 0x30
        uint32_t item_count;
        uint32_t eyecolor;
        uint32_t npc_speed;
        uint32_t particle_size_alt;
    };
    uint32_t data_size;
    uint32_t data; //not officially a part of gameupdatepacket, thus sizeof(GameUpdatePacket) is 60 and not 56


};
struct gameupdatepacket_t {
    uint8_t m_type;
    uint8_t m_netid;
    uint8_t m_jump_amount;
    uint8_t m_count;
    int32_t m_player_flags;
    int32_t m_item;
    int32_t m_packet_flags;
    float m_struct_flags;
    int32_t m_int_data;
    float m_vec_x;
    float m_vec_y;
    float m_vec2_x;
    float m_vec2_y;
    float m_particle_time;
    uint32_t m_state1;
    uint32_t m_state2;
    uint32_t m_data_size;
    uint32_t m_data;
};
typedef struct gametankpacket_t {
    int32_t m_type;
    char m_data;
} gametextpacket_t;
#pragma pack(pop)
class vector2_t {
public:
    float m_x;
    float m_y;
    vector2_t() : m_x(0), m_y(0) {}
    vector2_t(float x, float y) : m_x(x), m_y(y) {}
    bool operator==(vector2_t& rhs) { return m_x == rhs.m_x && m_y == rhs.m_y; }
    bool operator==(const vector2_t& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y; }
    vector2_t operator+(const vector2_t& rhs) { return vector2_t(m_x + rhs.m_x, m_y + rhs.m_y); }
    vector2_t operator-(const vector2_t& rhs) { return vector2_t(m_x - rhs.m_x, m_y - rhs.m_y); }
    float distance(float x, float y) {
        float value_x = this->m_x - x;
        float value_y = this->m_y - y;
        return sqrt(value_x * value_x + value_y * value_y);
    }
    void set(float x, float y) {
        m_x = (x), m_y = (y);
    }
    void print() {
        printf("X : %.3f Y : %.3f\n", m_x, m_y);
    }
};
class vector2i_t {
public:
    int m_x;
    int m_y;
    vector2i_t() : m_x(0), m_y(0) {}
    vector2i_t(int x, int y) : m_x(x), m_y(y) {}
    vector2i_t(uint32_t x, uint32_t y) : m_x(x), m_y(y) {}
    bool operator==(vector2i_t& rhs) { return m_x == rhs.m_x && m_y == rhs.m_y; }
    bool operator!=(vector2i_t& rhs) {
        return m_x != rhs.m_x || m_y != rhs.m_y;
    }
    bool operator==(const vector2i_t& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y; }
    vector2i_t operator+(const vector2i_t& rhs) { return vector2i_t(m_x + rhs.m_x, m_y + rhs.m_y); }
    vector2i_t operator-(const vector2i_t& rhs) { return vector2i_t(m_x - rhs.m_x, m_y - rhs.m_y); }
    float distance(int x, int y) {
        float value_x = float(this->m_x) - x;
        float value_y = float(this->m_y) - y;
        return sqrt(value_x * value_x + value_y * value_y);
    }
};
class vector3_t {
public:
    float m_x;
    float m_y;
    float m_z;
    vector3_t() : m_x(0), m_y(0), m_z(0) {}
    vector3_t(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
    bool operator==(vector3_t& rhs) { return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z; }
    bool operator==(const vector3_t& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z; }
    vector3_t operator+(const vector3_t& rhs) { return vector3_t(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z); }
    vector3_t operator-(const vector3_t& rhs) { return vector3_t(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z); }
};
class rect_t {
public:
    float m_x;
    float m_y;
    float m_w;
    float m_h;
    rect_t() : m_x(0), m_y(0), m_w(0), m_h(0) {}
    rect_t(float x, float y, float w, float h) : m_x(x), m_y(y), m_w(w), m_h(h) {}
    bool operator==(rect_t& rhs) { return m_x == rhs.m_x && m_y == rhs.m_y && m_w == rhs.m_w && m_h == rhs.m_h; }
    bool operator==(const rect_t& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y && m_w == rhs.m_w && m_h == rhs.m_h; }
    rect_t operator+(const rect_t& rhs) { return rect_t(m_x + rhs.m_x, m_y + rhs.m_y, m_w + rhs.m_w, m_h + rhs.m_h); }
    rect_t operator-(const rect_t& rhs) { return rect_t(m_x - rhs.m_x, m_y - rhs.m_y, m_w - rhs.m_w, m_h - rhs.m_h); }
};
enum {
    PACKET_STATE = 0,
    PACKET_CALL_FUNCTION,
    PACKET_UPDATE_STATUS,
    PACKET_TILE_CHANGE_REQUEST,
    PACKET_SEND_MAP_DATA,
    PACKET_SEND_TILE_UPDATE_DATA,
    PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE,
    PACKET_TILE_ACTIVATE_REQUEST,
    PACKET_TILE_APPLY_DAMAGE,
    PACKET_SEND_INVENTORY_STATE,
    PACKET_ITEM_ACTIVATE_REQUEST,
    PACKET_ITEM_ACTIVATE_OBJECT_REQUEST,
    PACKET_SEND_TILE_TREE_STATE,
    PACKET_MODIFY_ITEM_INVENTORY,
    PACKET_ITEM_CHANGE_OBJECT,
    PACKET_SEND_LOCK,
    PACKET_SEND_ITEM_DATABASE_DATA,
    PACKET_SEND_PARTICLE_EFFECT,
    PACKET_SET_ICON_STATE,
    PACKET_ITEM_EFFECT,
    PACKET_SET_CHARACTER_STATE,
    PACKET_PING_REPLY,
    PACKET_PING_REQUEST,
    PACKET_GOT_PUNCHED,
    PACKET_APP_CHECK_RESPONSE,
    PACKET_APP_INTEGRITY_FAIL,
    PACKET_DISCONNECT,
    PACKET_BATTLE_JOIN,
    PACKET_BATTLE_EVEN,
    PACKET_USE_DOOR,
    PACKET_SEND_PARENTAL,
    PACKET_GONE_FISHIN,
    PACKET_STEAM,
    PACKET_PET_BATTLE,
    PACKET_NPC,
    PACKET_SPECIAL,
    PACKET_SEND_PARTICLE_EFFECT_V2,
    GAME_ACTIVE_ARROW_TO_ITEM,
    GAME_SELECT_TILE_INDEX
};
enum {
    NET_MESSAGE_UNKNOWN = 0,
    NET_MESSAGE_SERVER_HELLO,
    NET_MESSAGE_GENERIC_TEXT,
    NET_MESSAGE_GAME_MESSAGE,
    NET_MESSAGE_GAME_PACKET,
    NET_MESSAGE_ERROR,
    NET_MESSAGE_TRACK,
    NET_MESSAGE_CLIENT_LOG_REQUEST,
    NET_MESSAGE_CLIENT_LOG_RESPONSE,
};
namespace utils {
    void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
    bool is_number(const std::string& s);
    std::string hex_str(unsigned char data);
    INLINE char* get_text(ENetPacket* packet) {
        gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
        memset(packet->data + packet->dataLength - 1, 0, 1);
        return static_cast<char*>(&tank->m_data);
    }
    INLINE GameUpdatePacket* get_structs(ENetPacket* packet) {
        if (packet->dataLength < sizeof(GameUpdatePacket) - 4)
            return nullptr;
        GameUpdatePacket* tank = reinterpret_cast<GameUpdatePacket*>(packet->data);
        GameUpdatePacket* gamepacket = reinterpret_cast<GameUpdatePacket*>(packet->data + 4);
        if (gamepacket->flags & 8) {
            if (packet->dataLength < gamepacket->data_size + 60) {
                printf("got invalid packet. (too small)\n");
                return nullptr;
            }
            return reinterpret_cast<GameUpdatePacket*>(&tank->data);
        }
        else
            gamepacket->data_size = 0;


        return gamepacket;
    }
    INLINE gameupdatepacket_t* get_struct(ENetPacket* packet) {
        if (packet->dataLength > sizeof(gameupdatepacket_t) - 4) {
            gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
            gameupdatepacket_t* gamepacket = reinterpret_cast<gameupdatepacket_t*>(packet->data + 4);
            if (gamepacket->m_packet_flags & 8) {

                return (packet->dataLength > gamepacket->m_data_size + 60 ? reinterpret_cast<gameupdatepacket_t*>(&tank->m_data) : nullptr);
            }
            else
                gamepacket->m_data_size = 0;
            return gamepacket;
        }
        return nullptr;
    }
    INLINE std::string random_color()
    {

        srand((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());


        size_t length = 1;
        auto randchar = []() -> char
        {
            const char charset[] =
                "0123456789"
                "bwpo^$#@!qecra&s";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[rand() % max_index];
        };
        std::string str(length, 0);
        std::generate_n(str.begin(), length, randchar);
        return str;
    }
    INLINE int random(int min, int max, std::mt19937* value= nullptr) {
        if (value == nullptr) {
            std::mt19937 rng;
            rng.seed((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(rng);
        }
        else
        {
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(*value);
        }

    }
    INLINE std::string generate_rid() {
        std::string rid_str;

        for (int i = 0; i < 16; i++)
            rid_str += utils::hex_str(random(0, 255));

        std::transform(rid_str.begin(), rid_str.end(), rid_str.begin(), ::toupper);

        return rid_str;
    }
    INLINE uint32_t hash(uint8_t* str, uint32_t len) {
        if (!str)
            return 0;
        uint8_t* n = (uint8_t*)str;
        uint32_t acc = 0x55555555;
        if (!len)
            while (*n)
                acc = (acc >> 27) + (acc << 5) + *n++;
        else
            for (uint32_t i = 0; i < len; i++)
                acc = (acc >> 27) + (acc << 5) + *n++;
        return acc;
    }
    INLINE std::string generate_mac(const std::string& prefix) {
        std::string x = prefix + ":";
        for (int i = 0; i < 5; i++) {
            x += utils::hex_str(random(0, 255));
            if (i != 4)
                x += ":";
        }
        return x;
    }
    const char hexmap_s[17] = "0123456789abcdef";
    INLINE std::string hex_str(unsigned char data) {
        std::string s(2, ' ');
        s[0] = hexmap_s[(data & 0xF0) >> 4];
        s[1] = hexmap_s[data & 0x0F];
        return s;
    }
    INLINE int ExtractINT(std::string value)
    {
        int valueof;
        std::string tempForINT;
        for (char& c : value) {
            if (is_number(std::string{ c }))
                tempForINT.append(std::string{ c });
        }

        return std::atoi(tempForINT.c_str());
    }
    INLINE std::string random(uint32_t length) {
        static auto randchar = []() -> char {
            const char charset[] =
                "0123456789"
                "qwertyuiopasdfghjklzxcvbnm"
                "QWERTYUIOPASDFGHJKLZXCVBNM";
            const uint32_t max_index = (sizeof(charset) - 1);
            return charset[random(INT16_MAX, INT32_MAX) % max_index];
        };

        std::string str(length, 0);
        std::generate_n(str.begin(), length, randchar);
        return str;
    }
    INLINE void DeleteWhiteSpice(std::string& str) {
        std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
        str.erase(end_pos, str.end());

    }
    INLINE void DeleteNewLine(std::string& str) {
        ReplaceAll(str, "\n", "");

    }
    INLINE void getplayername(std::string& str) {
        str.erase(0, 2);
        str.erase(str.size() - 2);
    }
    INLINE void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }

    }
    INLINE std::vector<std::string>explode(const std::string& delimiter, const std::string& str)
    {
        std::vector<std::string> arr{};
        const int strleng = str.length();
        const int delleng = delimiter.length();
        if (delleng == 0)
            return arr;
        int i = 0;
        int k = 0;
        while (i < strleng)
        {
            int j = 0;
            while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
                j++;
            if (j == delleng)
            {
                arr.push_back(str.substr(k, i - k));
                i += delleng;
                k = i;
            }
            else
            {
                i++;
            }
        }
        arr.push_back(str.substr(k, i - k));
        return arr;
    }
    INLINE bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }
    INLINE bool is_number(const std::string& s) {
        return !s.empty() && std::find_if(s.begin() + (*s.data() == '-' ? 1 : 0), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    }
    std::vector<std::string_view> StringTokenize(const std::string_view& strv, char seperator = '|');
    INLINE std::vector<std::string_view> StringTokenize(const std::string_view& strv, char seperator)
    {
        std::vector<std::string_view> output;
        size_t first = 0;

        while (first < strv.size())
        {
            const size_t second = strv.find_first_of(seperator, first);

            if (first != second)
                output.emplace_back(strv.substr(first, second - first));

            if (second == std::string_view::npos)
                break;

            first = second + 1;
        }

        return output;
    }
    INLINE std::string getTemp()
    {
#ifdef _WIN32
        if (std::string{ getenv("TEMP") }.size() > 0)
            return  getenv("TEMP");
        else if (std::filesystem::temp_directory_path().string().size() > 0)
             return std::filesystem::temp_directory_path().string();
        else
            return "";
#endif
        return "";
    }
    INLINE bool writeFile(std::string Location, std::string Content)
    {
        std::ofstream Writer(Location);
        if (!Writer) {
            std::cerr << "Error: file could not be opened" << std::endl;
            return false;
        }
        else
            Writer << Content;
        Writer.close();
        return true;
    }
    INLINE std::string readFile(std::string Location)
    {
       std::ifstream Reader(Location);
       if (Reader)
        {
            std::string str((std::istreambuf_iterator<char>(Reader)),
                std::istreambuf_iterator<char>());
            return str;
            Reader.close();
        }
        else
            return "";
    }
}
class rtvar {
public:
    class pair {
    public:
        std::string m_key{};
        std::vector<std::string> m_values{};
        std::string m_value;
        pair() {
        }
        pair(std::string key, std::initializer_list<std::string> values) : m_key(key), m_values(values) {
        }
        bool operator==(const rtvar::pair& rst) {
            return m_key == rst.m_key && m_values[0] == rst.m_values[0];
        }
        static pair parse(std::string str) {
            pair ret{};
            if (!str.length()) { // we dont want to parse empty string so m_pairs vector wont make the app crash
                ret.m_value.append("[EMPTY]");
                ret.m_values.push_back("[EMPTY]");
                return ret;
            }
            auto g = utils::explode("|", str);
            ret.m_key = (g.size() > 0) ? g[0] : "";
            ret.m_value = (g.size() >= 2) ? g[1] : "";


            if (g.size() > 2)
            {
                for (unsigned i = 1; i < g.size(); ++i) {
                    ret.m_values.push_back(g[i]);
                }


            }
            else
                ret.m_values.push_back((g.size() >= 2) ? g[1] : "");

            return ret;




        }
        std::string serialize() {
            std::string ret{};
            ret.append(m_key);
            for (auto& val : m_values) {
                ret.append("|");
                ret.append(val);
            }
            return ret;
        }
    };
    rtvar() {
    }
    rtvar(std::initializer_list<pair> pairs) : m_pairs(pairs) {
    }
    static rtvar parse(std::string str) {
        rtvar ret{};
        std::stringstream ss(str);
        std::string token{};

        while (std::getline(ss, token, '\n'))
            ret.append(token);
        return ret;
    }
    static rtvar parseALT(std::string str) {
        rtvar ret{};
        std::stringstream ss(str);
        std::string token{};
        auto g = utils::explode("|", str);
        for (auto xx : g)
        {
            utils::ReplaceAll(xx, "=", "|");
            ret.append(xx);
        }

        return ret;
    }
    pair& append(std::string str) {
        pair p = pair::parse(str);
        m_pairs.push_back(p);
        return m_pairs.back();
    }
    pair& get(int i) {
        if ((unsigned)i >= m_pairs.size())
            return m_pairs[0];
        return m_pairs[i];
    }
    bool valid() {
        if (m_pairs.size() < 1)
            return false;

        if (m_pairs[0].m_values.size() < 1)
            return false;

        return true;
    }
    pair* find(const std::string& key) {
        int inx = 0;
        for (auto pair : m_pairs) {
            if (pair.m_key == key) //we cant return local pairs addr
                return &m_pairs[inx];
            inx++;
        }
        return nullptr;
    }

    std::string get(const std::string& key) {
        auto pair = find(key);
        if (pair) {
            std::string now = pair->m_value;
            return now;
        }
        return "";
    }
    void set(const std::string& key, std::string value) {
        auto pair = find(key);
        if (pair && pair->m_values.size() >= 1)
            pair->m_values[0] = value;
    }
    std::string serialize() {
        std::string ret{};
        for (auto& val : m_pairs) {
            ret.append(val.serialize());
            ret.append("\n");
        }
        if (ret != "")
            ret.erase(ret.end());
        return ret;
    }
    bool validate_ints(std::vector<std::string> vals) {
        for (auto str : vals) {
            auto pair = this->find(str);
            if (!pair)
                return false;
            if (!utils::is_number(pair->m_value))
                return false;
        }
        return true;
    }
    bool validate_int(std::string str) {
        auto pair = this->find(str);
        if (!pair)
            return false;
        if (!utils::is_number(pair->m_value))
            return false;
        return true;
    }
    inline int get_int(const std::string& key) { 
        auto pair = find(key);
        if (pair)
            return atoi(pair->m_value.c_str());
        return 0;
    }
    inline long long get_long(const std::string& key) { //assumes validate_ints
        return atoll(find(key)->m_value.c_str());
    }
    size_t size() const {
        return m_pairs.size();
    }
    void remove(const std::string& key) {
        auto pair = find(key);
        if (pair) {
            auto& ref = *pair;
            m_pairs.erase(std::remove(m_pairs.begin(), m_pairs.end(), ref), m_pairs.end());
        }
    }

private:



    //i could use std::map but for the sake of simplicity i dont, i want the code to be as readable as possible
    std::vector<pair> m_pairs{};
};
class rtvar_opt { //optimized version of rtvars (really nothing more than a container) when only needing to append strings
private:
    std::string m_var{};

public:
    rtvar_opt() {
    }
    rtvar_opt(std::string start) {
        m_var = start;
    }
    void append(std::string str) {
        m_var = m_var.append("\n" + str);
    }
    std::string get() {
        return m_var;
    }
};
class variant_t {
public:
    enum class vartype_t { TYPE_UNUSED, TYPE_FLOAT, TYPE_STRING, TYPE_VECTOR2, TYPE_VECTOR3, TYPE_UINT32, TYPE_ENTITY, TYPE_COMPONENT, TYPE_RECT, TYPE_INT32 };
    variant_t() { set_defaults(); }
    variant_t(const variant_t& v) {
        set_defaults();
        *this = v;
    }
    variant_t(int32_t var) {
        set_defaults();
        set(var);
    }
    variant_t(uint32_t var) {
        set_defaults();
        set(var);
    }
    variant_t(float var) {
        set_defaults();
        set(var);
    }
    variant_t(float x, float y) {
        set_defaults();
        set(vector2_t(x, y));
    }
    variant_t(float x, float y, float z) {
        set_defaults();
        set(vector3_t(x, y, z));
    }
    variant_t(const vector2_t& v2) {
        set_defaults();
        set(v2);
    }
    variant_t(const vector3_t& v3) {
        set_defaults();
        set(v3);
    }
    variant_t(const rect_t& r) {
        set_defaults();
        set(r);
    }
    variant_t(const std::string& var) {
        set_defaults();
        set(var);
    }

    void reset() { m_type = vartype_t::TYPE_UNUSED; }
    void set(const variant_t& v) {
        switch (v.get_type()) {
        case vartype_t::TYPE_FLOAT: set(v.get_float()); break;
        case vartype_t::TYPE_STRING: set(v.get_string()); break;
        case vartype_t::TYPE_VECTOR2: set(v.get_vector2()); break;
        case vartype_t::TYPE_VECTOR3: set(v.get_vector3()); break;
        case vartype_t::TYPE_UINT32: set(v.get_uint32()); break;
        case vartype_t::TYPE_INT32: set(v.get_int32()); break;
        case vartype_t::TYPE_RECT: set(v.get_rect()); break;
        case vartype_t::TYPE_ENTITY:
        case vartype_t::TYPE_COMPONENT:
        default: break;
        }
    }
    void set(float var) {
        m_type = vartype_t::TYPE_FLOAT;
        *((float*)m_var) = var;
    }
    void set(uint32_t var) {
        m_type = vartype_t::TYPE_UINT32;
        *((uint32_t*)m_var) = var;
    }
    void set(int32_t var) {
        m_type = vartype_t::TYPE_INT32;
        *((int32_t*)m_var) = var;
    }
    void operator=(float var) {
        set(var);
    }
    void operator=(int32_t var) {
        set(var);
    }
    void operator=(uint32_t var) {
        set(var);
    }
    void operator=(std::string const& var) {
        set(var);
    }
    void set(std::string const& var) {
        m_type = vartype_t::TYPE_STRING;
        m_string = var;
    }
    void operator=(vector2_t const& var) {
        set(var);
    }
    void set(vector2_t const& var) {
        m_type = vartype_t::TYPE_VECTOR2;
        *((vector2_t*)m_var) = var;
    }
    void set(float x, float y) { set(vector2_t(x, y)); }
    void operator=(vector3_t const& var) {
        set(var);
    }
    void operator=(rect_t const& var) {
        set(var);
    }
    void set(vector3_t const& var) {
        m_type = vartype_t::TYPE_VECTOR3;
        *((vector3_t*)m_var) = var;
    }
    void set(rect_t const& var) {
        m_type = vartype_t::TYPE_RECT;
        *((rect_t*)m_var) = var;
    }
    void set(float x, float y, float z) { set(vector3_t(x, y, z)); }

    float& get_float() {
        if (m_type == vartype_t::TYPE_UNUSED)
            set(float(0));
        return *((float*)m_var);
    }
    int32_t& get_int32() {
        if (m_type == vartype_t::TYPE_UNUSED)
            set(int32_t(0));
        return *((int32_t*)m_var);
    }
    uint32_t& get_uint32() {
        if (m_type == vartype_t::TYPE_UNUSED)
            set(uint32_t(0));
        return *((uint32_t*)m_var);
    }
    std::string& get_string() { return m_string; }
    vector2_t& get_vector2() {
        if (m_type == vartype_t::TYPE_UNUSED)
            set(vector2_t(0, 0));
        return *((vector2_t*)m_var);
    }
    vector3_t& get_vector3() {
        if (m_type == vartype_t::TYPE_UNUSED)
            set(vector3_t(0, 0, 0));
        return *((vector3_t*)m_var);
    }
    rect_t& get_rect() {
        if (m_type == vartype_t::TYPE_UNUSED)
            set(rect_t(0, 0, 0, 0));
        return *((rect_t*)m_var);
    }

    const float& get_float() const { return *((float*)m_var); }
    const int32_t& get_int32() const { return *((int32_t*)m_var); }
    const uint32_t& get_uint32() const { return *((uint32_t*)m_var); }
    const std::string& get_string() const { return m_string; }
    const vector2_t& get_vector2() const { return *((vector2_t*)m_var); }
    const vector3_t& get_vector3() const { return *((vector3_t*)m_var); }
    const rect_t& get_rect() const { return *((rect_t*)m_var); }

    vartype_t get_type() const { return m_type; }

    std::string print() {
        switch (get_type()) {
        case vartype_t::TYPE_FLOAT: return std::to_string(get_float());
        case vartype_t::TYPE_STRING: return get_string();
        case vartype_t::TYPE_VECTOR2: return "x: " + std::to_string(get_vector2().m_x) + " y: " + std::to_string(get_vector2().m_y);
        case vartype_t::TYPE_VECTOR3:
            return "x: " + std::to_string(get_vector3().m_x) + " y: " + std::to_string(get_vector3().m_y) + " z: " + std::to_string(get_vector3().m_z);
        case vartype_t::TYPE_UINT32: return std::to_string(get_uint32());
        case vartype_t::TYPE_INT32: return std::to_string(get_int32());
        case vartype_t::TYPE_RECT:
            return "x: " + std::to_string(get_rect().m_x) + " y: " + std::to_string(get_rect().m_y) + " w: " + std::to_string(get_rect().m_w) +
                " h: " + std::to_string(get_rect().m_h);
        case vartype_t::TYPE_ENTITY:
        case vartype_t::TYPE_COMPONENT:
        case vartype_t::TYPE_UNUSED: return "unused"; break;
        default: break;
        }
        return "unknown";
    }
    variant_t& operator=(const variant_t& rhs) {
        m_type = rhs.m_type;
        m_pointer = rhs.m_pointer;
        memcpy(m_var, rhs.m_var, C_VAR_SPACE_BYTES);
        m_string = rhs.m_string;
        return *this;
    }
    variant_t& operator+=(const variant_t& rhs) {
        if (get_type() == rhs.get_type()) {
            switch (get_type()) {
            case vartype_t::TYPE_FLOAT: set(get_float() + rhs.get_float()); break;
            case vartype_t::TYPE_STRING: set(get_string() + rhs.get_string()); break;
            case vartype_t::TYPE_VECTOR2: set(get_vector2() + rhs.get_vector2()); break;
            case vartype_t::TYPE_VECTOR3: set(get_vector3() + rhs.get_vector3()); break;
            case vartype_t::TYPE_UINT32: set(get_uint32() + rhs.get_uint32()); break;
            case vartype_t::TYPE_INT32: set(get_int32() + rhs.get_int32()); break;
            default: break;
            }
        }
        return *this;
    }
    variant_t& operator-=(const variant_t& rhs) {
        if (get_type() == rhs.get_type()) {
            switch (get_type()) {
            case vartype_t::TYPE_FLOAT: set(get_float() - rhs.get_float()); break;
            case vartype_t::TYPE_VECTOR2: set(get_vector2() - rhs.get_vector2()); break;
            case vartype_t::TYPE_VECTOR3: set(get_vector3() - rhs.get_vector3()); break;
            case vartype_t::TYPE_UINT32: set(get_uint32() - rhs.get_uint32()); break;
            case vartype_t::TYPE_INT32: set(get_int32() - rhs.get_int32()); break;
            default: break;
            }
        }
        return *this;
    }
    bool operator==(const variant_t& rhs) const {
        if (get_type() != rhs.get_type())
            return false;
        switch (get_type()) {
        case vartype_t::TYPE_UNUSED: return true;
        case vartype_t::TYPE_FLOAT: return get_float() == rhs.get_float();
        case vartype_t::TYPE_STRING: return get_string() == rhs.get_string();
        case vartype_t::TYPE_VECTOR2: return get_vector2() == rhs.get_vector2();
        case vartype_t::TYPE_VECTOR3: return get_vector3() == rhs.get_vector3();
        case vartype_t::TYPE_UINT32: return get_uint32() == rhs.get_uint32();
        case vartype_t::TYPE_RECT: return get_rect() == rhs.get_rect();
        case vartype_t::TYPE_INT32: return get_int32() == rhs.get_int32();
        case vartype_t::TYPE_ENTITY:
        case vartype_t::TYPE_COMPONENT:
        default: return false;
        }
    }
    bool operator!=(const variant_t& rhs) const { return !operator==(rhs); }
    friend class variantlist_t;

private:
    void set_defaults() { m_type = vartype_t::TYPE_UNUSED; }
    vartype_t m_type;
    void* m_pointer;
    union {
        uint8_t m_var[C_VAR_SPACE_BYTES];
        float m_as_floats[4];
        uint32_t m_as_uint32s[4];
        int32_t m_as_int32s[4];
    };
    std::string m_string;
};
inline variant_t operator+(variant_t lhs, const variant_t& rhs) {
    lhs += rhs;
    return lhs;
}
inline variant_t operator-(variant_t lhs, const variant_t& rhs) {
    lhs -= rhs;
    return lhs;
}
#define C_MAX_VARIANT_LIST_PARMS 7
class variantlist_t {
    int size_of_variant(variant_t::vartype_t type) {
        switch (type) {
        case variant_t::vartype_t::TYPE_UNUSED:
        case variant_t::vartype_t::TYPE_COMPONENT:
        case variant_t::vartype_t::TYPE_ENTITY: return 0;
        case variant_t::vartype_t::TYPE_UINT32:
        case variant_t::vartype_t::TYPE_INT32:
        case variant_t::vartype_t::TYPE_FLOAT: return 4;
        case variant_t::vartype_t::TYPE_VECTOR2: return sizeof(vector2_t);
        case variant_t::vartype_t::TYPE_VECTOR3: return sizeof(vector3_t);
        case variant_t::vartype_t::TYPE_RECT: return sizeof(rect_t);
        default: return 0;
        }
    }

public:
    variantlist_t() {};
    variant_t& get(int parmNum) { return m_variant[parmNum]; }
    variant_t& operator[](int num) {
        return m_variant[num];
    }
    variantlist_t(const std::string& string) {
        m_variant[0] = variant_t(string);
    }
    variantlist_t(variant_t v0) { m_variant[0] = v0; }
    variantlist_t(variant_t v0, variant_t v1) {
        m_variant[0] = v0;
        m_variant[1] = v1;
    }
    variantlist_t(variant_t v0, variant_t v1, variant_t v2) {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
    }
    variantlist_t(variant_t v0, variant_t v1, variant_t v2, variant_t v3) {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
        m_variant[3] = v3;
    }
    variantlist_t(variant_t v0, variant_t v1, variant_t v2, variant_t v3, variant_t v4) {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
        m_variant[3] = v3;
        m_variant[4] = v4;
    }
    variantlist_t(variant_t v0, variant_t v1, variant_t v2, variant_t v3, variant_t v4, variant_t v5) {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
        m_variant[3] = v3;
        m_variant[4] = v4;
        m_variant[5] = v5;
    }
    void reset() {
        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++) m_variant[i].reset();
    }
    uint32_t get_mem_needed() {
        int vars_used = 0;
        int mem_needed = 0;
        int var_size;
        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++) {
            if (m_variant[i].get_type() == variant_t::vartype_t::TYPE_STRING)
                var_size = m_variant[i].get_string().size() + 4;
            else
                var_size = size_of_variant(m_variant[i].get_type());
            if (var_size > 0) {
                vars_used++;
                mem_needed += var_size;
            }
        }
        int total = mem_needed + 1 + (vars_used * 2);
        return total;
    }
    uint8_t* serialize_to_mem(uint32_t* size, uint8_t* data) {
        int vars_used = 0;
        int mem_needed = 0;
        int var_size;
        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++) {
            if (m_variant[i].get_type() == variant_t::vartype_t::TYPE_STRING)
                var_size = m_variant[i].get_string().size() + 4;
            else
                var_size = size_of_variant(m_variant[i].get_type());
            if (var_size > 0) {
                vars_used++;
                mem_needed += var_size;
            }
        }
        int total = mem_needed + 1 + (vars_used * 2);
        if (!data)
            data = new uint8_t[total];
        uint8_t* p = data;
        *(p++) = uint8_t(vars_used);
        for (int idx = 0; idx < C_MAX_VARIANT_LIST_PARMS; idx++) {
            uint8_t type = uint8_t(m_variant[idx].get_type());
            if (m_variant[idx].get_type() == variant_t::vartype_t::TYPE_STRING) {
                uint32_t len = m_variant[idx].get_string().size();
                memcpy(p++, &idx, 1);
                memcpy(p++, &type, 1);
                memcpy(p, &len, 4);
                p += 4;
                memcpy(p, m_variant[idx].get_string().c_str(), len);
                p += len;
            }
            else {
                var_size = size_of_variant(m_variant[idx].get_type());
                if (var_size > 0) {
                    memcpy(p++, &idx, 1);
                    memcpy(p++, &type, 1);
                    memcpy(p, m_variant[idx].m_var, var_size);
                    p += var_size;
                }
            }
        }
        if (size)
            *size = total;
        return data;
    }
    bool serialize_from_mem(uint8_t* data, int* read = 0) { //robinson way cuz i dont want to refactor this yet
        uint8_t* p = data;
        uint8_t count = *(p++);
        for (int i = 0; i < count; i++) {
            uint8_t index = *(p++);
            uint8_t type = *(p++);
            switch (variant_t::vartype_t(type)) {
            case variant_t::vartype_t::TYPE_STRING: {
                uint32_t len;
                memcpy(&len, p, 4);
                p += 4;
                std::string v;
                v.resize(len);
                memcpy(&v[0], p, len);
                p += len;
                m_variant[index].set(v);
                break;
            }
            case variant_t::vartype_t::TYPE_UINT32: {
                uint32_t v;
                memcpy(&v, p, sizeof(uint32_t));
                p += sizeof(uint32_t);
                m_variant[index].set(v);
                break;
            }
            case variant_t::vartype_t::TYPE_INT32: {
                int32_t v;
                memcpy(&v, p, sizeof(int32_t));
                p += sizeof(int32_t);
                m_variant[index].set(v);
                break;
            }
            case variant_t::vartype_t::TYPE_FLOAT: {
                float v;
                memcpy(&v, p, sizeof(float));
                p += sizeof(float);
                m_variant[index].set(v);
                break;
            }
            case variant_t::vartype_t::TYPE_VECTOR2: {
                vector2_t v;
                memcpy(&v, p, sizeof(vector2_t));
                p += sizeof(vector2_t);
                m_variant[index].set(v);
                break;
            }
            case variant_t::vartype_t::TYPE_VECTOR3: {
                vector3_t v;
                memcpy(&v, p, sizeof(vector3_t));
                p += sizeof(vector3_t);
                m_variant[index].set(v);
                break;
            }
            case variant_t::vartype_t::TYPE_RECT: {
                rect_t v;
                memcpy(&v, p, sizeof(rect_t));
                p += sizeof(rect_t);
                m_variant[index].set(v);
                break;
            }
            default:
                if (read)
                    *read = 0;
                return false;
            }
        }
        if (read)
            *read = int(p - data);
        return true;
    }
    variant_t m_variant[C_MAX_VARIANT_LIST_PARMS];
    int Net_ID, Delay_ = 0;
    int getnetid(int netid = 0)
    {
        if (netid != 0)
            return Net_ID;
        Net_ID = netid;

        return Net_ID;
    }
    int getDelay(int Delay = 0)
    {
        if (Delay_ != 0)
            return Delay_;
        Delay_ = Delay;

        return Delay_;
    }
    std::string print() {
        std::stringstream ss;
        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++) {
            if (m_variant[i].get_type() == variant_t::vartype_t::TYPE_UNUSED)
                continue;
            ss << "param " << std::to_string(i) << ": " << m_variant[i].print() + "\n";
        }
        if (ss.str().empty())
            ss.str("(none)");
        return ss.str();
    }
    std::string GetValue(int val = 0) {
        if (m_variant[val].get_type() != variant_t::vartype_t::TYPE_UNUSED)
            return m_variant[val].print();

        return "";
    }

    std::string Var_0() {
        if (m_variant[0].get_type() != variant_t::vartype_t::TYPE_UNUSED)
            return m_variant[0].print();

        return "";
    }
    std::string Var_1() {
        if (m_variant[1].get_type() != variant_t::vartype_t::TYPE_UNUSED)
            return m_variant[1].print();

        return "";
    }
    std::string Var_2() {
        if (m_variant[2].get_type() != variant_t::vartype_t::TYPE_UNUSED)
            return m_variant[2].print();

        return "";
    }
    std::string Var_3() {
        if (m_variant[3].get_type() != variant_t::vartype_t::TYPE_UNUSED)
            return m_variant[3].print();

        return "";
    }
    std::string Var_4() {
        if (m_variant[4].get_type() != variant_t::vartype_t::TYPE_UNUSED)
            return m_variant[4].print();

        return "";
    }
    std::string Var_5() {
        if (m_variant[5].get_type() != variant_t::vartype_t::TYPE_UNUSED)
            return m_variant[5].print();

        return "";
    }
};
inline vector2i_t VC2I(vector2_t v) {
    return vector2i_t(static_cast<int>((v.m_x + 8.f) / 32.f), static_cast<int>((v.m_y + 8.f) / 32.f));
}
inline vector2i_t VF2I(vector2_t v) {
    return vector2i_t(static_cast<int>((v.m_x + 8.f) / 32.f), static_cast<int>((v.m_y + 8.f) / 32.f));
}
inline vector2i_t VF2I_S(vector2_t v) { //strict
    return vector2i_t(static_cast<int>((v.m_x + 8.f) / 32.f), static_cast<int>((v.m_y + 8.f) / 32.f));
}
inline vector2_t VI2F(vector2i_t v) {
    return vector2_t(static_cast<int>((v.m_x + 8.f) / 32.f), static_cast<int>((v.m_y + 8.f) / 32.f));
}
inline vector2_t VI2F(vector2_t v) {
    return vector2_t(static_cast<int>((v.m_x + 8.f) / 32.f), static_cast<int>((v.m_y + 8.f) / 32.f));
}
typedef struct Item {
    uint16_t id = -1;
    uint8_t count = -1;
    uint8_t type =-1;
}Item;
struct PlayerInventory {
    uint32_t slotCount;
    uint16_t itemCount;
    bool doesItemExist(uint32_t itemID) {
        return items.contains(itemID);
    }
    int getItemCount(uint32_t itemID) {
        return doesItemExist(itemID) ? items.find(itemID)->second.count : 0;
    }
    std::unordered_map<uint32_t, Item> items;
};
#endif