from PIL import Image
import sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'python3 {sys.argv[0]} infile')
        sys.exit(0)

    img = Image.open(sys.argv[1])
    if img.size == (20, 16):
        cell_offset_x = 0
        cell_offset_y = 0
    elif img.size == (23, 17):
        cell_offset_x = 1
        cell_offset_y = 1
    else:
        print('Invalid image size, needs to be 20x16 or 23x17')
        sys.exit(1)

    for cell_y in range(2):
        for cell_x in range(4):
            for y in range(8):
                v = 0
                for x in range(5):
                    px = (cell_x * 5) + (cell_x * cell_offset_x) + x
                    py = (cell_y * 8) + (cell_y * cell_offset_y) + y
                    v |= (img.getpixel((px, py)) == (0, 0, 0, 255)) << (4 - x)
                print(f'{v:#0{4}x}, ', end='')
            print('')

