#ifndef OAM_H
#define OAM_H
#include <array>
#include <cstdint>

static constexpr int oamSize = 64;
struct Sprite {
  int8_t y_pos;
  int8_t tile_index;
  int8_t attributes;
  int8_t x_pos;
  // Overload the insertion operator to output Sprite information
  friend std::ostream& operator<<(std::ostream& os, const Sprite& sprite) {
    os << "Sprite(x=" << static_cast<int>(sprite.x_pos) 
       << ", y=" << static_cast<int>(sprite.y_pos)
       << ", tile=" << static_cast<int>(sprite.tile_index)
       << ", attr=" << static_cast<int>(sprite.attributes) << ")";
    return os;
  }
};

struct OAM{
  std::array<Sprite, oamSize> sprites;
};

#endif // OAM_H