struct _World
{
    std::string name{};
    std::string latest{};
    uint32_t width;
    uint32_t height;
    uint32_t timer;
    vector2i_t maindoor;
    /*std::unordered_map<uint32_t, std::shared_ptr<Tile>> tiles;
    std::unordered_map<uint32_t, std::shared_ptr<Dropped>> objects;*/
};