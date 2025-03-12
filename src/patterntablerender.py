from PIL import Image
import argparse

TILE_SIZE = 8
TILE_BYTES = 16
PATTERN_TABLE_SIZE = 16 * 16
PATTERN_TABLE_OFFSET = 0x1000

palettes = {
    'GRAYSCALE': [(0,0,0), (85,85,85), (170,170,170), (255,255,255)],
    'RGB': [(0,0,0), (0,0,255), (0,255,0), (255,0,0)],
    'DK' : [(0,0,0), (69,67,230), (120,39,56), (255,255,255)]
}

def decode_tile(data, index):
    tile = [[0] * TILE_SIZE for i in range(TILE_SIZE)]
    start_addr = index * TILE_BYTES

    for y in range(TILE_SIZE):
        low_byte = data[start_addr + y]
        high_byte = data[start_addr + y + TILE_SIZE]

        for x in range(TILE_SIZE):
            low_bit = (low_byte >> (7 - x)) & 1
            high_bit = (high_byte >> (7 - x)) & 1
            tile[y][x] = low_bit + (high_bit << 1)

    return tile

def render_table(data, palette='GRAYSCALE'):
    img_width = 16 * TILE_SIZE * 2 # 2 pattern tables
    img_height = 16 * TILE_SIZE
    img = Image.new('RGB', (img_width, img_height), 'black')

    for i in range(PATTERN_TABLE_SIZE):
        left_tile = decode_tile(data, i)
        x = (i % 16) * TILE_SIZE
        y = (i // 16) * TILE_SIZE

        right_tile = decode_tile(data, i + PATTERN_TABLE_SIZE)

        for ty in range(TILE_SIZE):
            for tx in range(TILE_SIZE):
                img.putpixel((x + tx, y + ty), palettes[palette][left_tile[ty][tx]])
                img.putpixel((x + tx + 128, y + ty), palettes[palette][right_tile[ty][tx]])

    

    return img

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Render NES pattern table to an image.')
    parser.add_argument('pattern_table_file', help='Path to the pattern table file (should be 8KB)')
    parser.add_argument('output_image', help='Path to save the output image')
    parser.add_argument('-P', '--palette', default='GRAYSCALE', choices=palettes.keys(), help='Palette to use (default: GRAYSCALE)')
    
    args = parser.parse_args()

    with open(args.pattern_table_file, 'rb') as f:
        data = f.read()
    
    image = render_table(data, args.palette)
    image.save(args.output_image)
    print(f"Image saved to {args.output_image} with palette {args.palette}")
