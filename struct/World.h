struct _World
{
    std::string name{};
    std::unordered_map<int/*netID*/, player> Players;
};