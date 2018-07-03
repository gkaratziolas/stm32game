import re

source = "mech.c"

def downsample_24_to_16(colour_24):
    if type(colour_24) != str and type(colour_24) != int:
        colour_24 = colour_24.group(0) 
    if type(colour_24) == str:
        colour_24 = int(colour_24, 16)

    b_24 = colour_24 & 0x0000ff
    g_24 = (colour_24 & 0x00ff00) >> 8
    r_24 = (colour_24 & 0xff0000) >> 16

    b_16 = b_24 >> 3
    g_16 = g_24 >> 2
    r_16 = r_24 >> 3

    colour_16 = r_16 + (g_16<<5) + (b_16<<11)
    return hex(colour_16)

def downsample_file_24_to_16(path):
    with open(path, "r") as f:
        data = f.read()

    data = re.sub(r'0x[0-9a-f]{8}', downsample_24_to_16 ,data)
    with open("test.c", "w") as f:
        f.write(data)


downsample_file_24_to_16("mech.c")
print(downsample_24_to_16(0xffffff))