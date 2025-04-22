#include <array>
#include <cstdint>

inline constexpr oamSize = 64;
struct Sprite {
  int8_t y_pos;
  int8_t tile_index;
  int8_t attributes;
  int8_t x_pos;
};


struct OAM{
  std::array<Sprite, oamSize> sprites;
};

