#include "mbtilereader.h"
#include <iostream>
extern "C" {
    #include "tippecanoe/projection.h"
}


MBTileReader::MBTileReader(const std::string & filepath)
{
    this->filename = filepath;
    if (sqlite3_open(filepath.c_str(), &this->db) != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(this->db);
    }


}

TileData* MBTileReader::get_tile(int z, unsigned x, unsigned y)
{
    TileData* tileData;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(this->db, MBTileReader::sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s: select failed: %s\n", this->filename.c_str(), sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_int(stmt, 1, z);
    sqlite3_bind_int(stmt, 2, x);
    sqlite3_bind_int(stmt, 3, (1LL << z) - 1 - y);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int len = sqlite3_column_bytes(stmt, 0);
        const char *s = (const char *) sqlite3_column_blob(stmt, 0);

        /*if (z != oz) {
            fprintf(stderr, "%s: Warning: using tile %d/%u/%u instead of %d/%u/%u\n", fname, z, x, y, oz, ox, oy);
        }*/

        std::cerr << "LEN" << len << std::endl;
        auto data = std::string(s, len);
        tileData = new TileData(data, z, x, y);
    }

    sqlite3_finalize(stmt);
    return tileData;
}

/**
 * @brief MBTileReader::get_tiles_inside
 * @param zoom
 * @param minLon
 * @param minLat
 * @param maxLon
 * @param maxLat
 *
 * Returns list of x,y, zoom tuples of tiles inside provided coordinates
 * @return
 */
TileList MBTileReader::get_tiles_inside(
        float minLon, float minLat, float maxLon, float maxLat, int zoom) {
    TileList tiles;
    long long min_x_tile, min_y_tile, max_x_tile, max_y_tile;

    latlon2tile(maxLat, minLon, zoom, &min_x_tile, &min_y_tile);
    latlon2tile(minLat, maxLon, zoom, &max_x_tile, &max_y_tile);

    std::cout << "(" << min_x_tile << " , " << min_y_tile << ")" << std::endl;
    std::cout << "(" << max_x_tile << " , " << max_y_tile << ")" << std::endl;
    //For zoom 12 unsigned is enough since there are 2^12x2^12 tiles
    //but for zoom 16 there is 2^16 * 2^16 tiles
    for(unsigned x_tile=min_x_tile; x_tile <= max_x_tile; x_tile++) {
        for (unsigned y_tile=min_y_tile; y_tile <= max_y_tile; y_tile++) {
            tiles.push_back(std::make_tuple(x_tile, y_tile, zoom));
        }
    }
    return tiles;
}

const std::string MBTileReader::sql = "SELECT tile_data from tiles where zoom_level = ? and tile_column = ? and tile_row = ?;";


