#ifndef TILEDATA_H
#define TILEDATA_H

#include <unordered_map>
#include "tilelayer.hpp"
#include "generated/vector_tile.pb.hpp"



class TileData
{
public:
    TileData(mapnik::vector::tile &vector_tile);

private:
    std::unordered_map<std::string, std::unique_ptr<TileLayer>> layers;


};

#endif // TILEDATA_H
