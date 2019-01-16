#ifndef TRIGON_PALLET_H_
#define TRIGON_PALLET_H_

#include <map>
#include <string>
#include <vector>

enum PalletOrder { C, TL, TC, TR, ML, MC, MR, LL, LC, LR, G };

static const std::map<char, std::string> abrev_ = {
    {'r', "red"},    {'k', "pink"},  {'p', "purple"},      {'P', "deep-purple"},
    {'i', "indigo"}, {'b', "blue"},  {'B', "light-blue"},  {'c', "cyan"},
    {'t', "teal"},   {'g', "green"}, {'G', "light-green"}, {'l', "lime"},
    {'y', "yellow"}, {'a', "amber"}, {'o', "orange"},      {'O', "deep-orange"},
    {'n', "brown"},  {'w', "grey"},  {'W', "blue-grey"}};

static const std::map<std::string, std::vector<uint32_t>> colors_ = {
    {"red",
     {0xffebee, 0xffcdd2, 0xef9a9a, 0xe57373, 0xef5350, 0xf44336, 0xe53935,
      0xd32f2f, 0xc62828, 0xb71c1c}},
    {"pink",
     {0xfce4ec, 0xf8bbd0, 0xf48fb1, 0xf06292, 0xec407a, 0xe91e63, 0xd81b60,
      0xc2185b, 0xad1457, 0x880e4f}},
    {"purple",
     {0xf3e5f5, 0xe1bee7, 0xce93d8, 0xba68c8, 0xab47bc, 0x9c27b0, 0x8e24aa,
      0x7b1fa2, 0x6a1b9a, 0x4a148c}},
    {"deep-purple",
     {0xede5f4, 0xd1c4e9, 0xb39ddb, 0x9575cd, 0x7e57c2, 0x673ab7, 0x5e35b1,
      0x512da8, 0x4527a0, 0x311b92}},
    {"indigo",
     {0xe8eaf6, 0xc5cae9, 0x9fa8da, 0x7986cb, 0x5c6bc0, 0x3f51b5, 0x3949ab,
      0x303f9f, 0x283593, 0x1a237e}},
    {"blue",
     {0xe3f2fd, 0xbbdefb, 0x90caf9, 0x64b5f6, 0x42a5f5, 0x2196f3, 0x1e88e5,
      0x1976d2, 0x1565c0, 0x0d47a1}},
    {"light-blue",
     {0xe1f5fe, 0xb3e5fc, 0x81d4fa, 0x4fc3f7, 0x29b6f6, 0x03a9f4, 0x039be5,
      0x0288d1, 0x0277bd, 0x01579b}},
    {"cyan",
     {0xe0f7fa, 0xb2ebf2, 0x80deea, 0x4dd0e1, 0x26c6da, 0x00bcd4, 0x00acc1,
      0x0097a7, 0x00838f, 0x006064}},
    {"teal",
     {0xe0f2f1, 0xb2dfdb, 0x80cbc4, 0x4db6ac, 0x26a69a, 0x009688, 0x00897b,
      0x00796b, 0x00695c, 0x004d40}},
    {"green",
     {0xe8f5e9, 0xc8e6c9, 0xa5d6a7, 0x81c784, 0x66bb6a, 0x4caf50, 0x43a047,
      0x388e3c, 0x2e7d32, 0x1b5e20}},
    {"light-green",
     {0xf1f8e9, 0xdcedc8, 0xc5e1a5, 0xaed581, 0x9ccc65, 0x8bc34a, 0x7cb342,
      0x689f38, 0x558b2f, 0x33691e}},
    {"lime",
     {0xf9fbe7, 0xf0f4c3, 0xe6ee9c, 0xdce775, 0xd4e157, 0xcddc39, 0xc0ca33,
      0xafb42b, 0x9e9d24, 0x827717}},
    {"yellow",
     {0xfffde7, 0xfff9c4, 0xfff59d, 0xfff176, 0xffee58, 0xffeb3b, 0xfdd835,
      0xfbc02d, 0xf9a825, 0xf57f17}},
    {"amber",
     {0xfff8e1, 0xffecb3, 0xffe082, 0xffd54f, 0xffca28, 0xffc107, 0xffb300,
      0xffa000, 0xff8f00, 0xff6f00}},
    {"orange",
     {0xfff3e0, 0xffe0b2, 0xffcc80, 0xffb74d, 0xffa726, 0xff9800, 0xfb8c00,
      0xf57c00, 0xef6c00, 0xe65100}},
    {"deep-orange",
     {0xfbe9e7, 0xffccbc, 0xffab91, 0xff8a65, 0xff7043, 0xff5722, 0xf4511e,
      0xe64a19, 0xd84315, 0xbf360c}},
    {"brown",
     {0xefebe9, 0xd7ccc8, 0xbcaaa4, 0xa1887f, 0x8d6e63, 0x795548, 0x6d4c41,
      0x5d4037, 0x4e342e, 0x3e2723}},
    {"grey",
     {0xfafafa, 0xf5f5f5, 0xeeeeee, 0xe0e0e0, 0xbdbdbd, 0x9e9e9e, 0x757575,
      0x616161, 0x424242, 0x212121}},
    {"blue-grey",
     {0xeceff1, 0xcfd8dc, 0xb0bec5, 0x90a4ae, 0x78909c, 0x607d8b, 0x546e7a,
      0x455a64, 0x37474f, 0x263238}},
};

static const std::map<std::string, std::string> pallets_ = {
    {"red", "r0r1r2r3r4r5r6r7r8r9"},
    {"pink", "k0k1k2k3k4k5k6k7k8k9"},
    {"purple", "p0p1p2p3p4p5p6p7p8p9"},
    {"deep-purple", "P0P1P2P3P4P5P6P7P8P9"},
    {"indigo", "i0i1i2i3i4i5i6i7i8i9"},
    {"blue", "b0b1b2b3b4b5b6b7b8b9"},
    {"light-blue", "B0B1B2B3B4B5B6B7B8B9"},
    {"cyan", "c0c1c2c3c4c5c6c7c8c9"},
    {"teal", "t0t1t2t3t4t5t6t7t8t9"},
    {"green", "g0g1g2g3g4g5g6g7g8g9"},
    {"light-green", "G0G1G2G3G4G5G6G7G8G9"},
    {"lime", "l0l1l2l3l4l5l6l7l8l9"},
    {"yellow", "y0y1y2y3y4y5y6y7y8y9"},
    {"amber", "a0a1a2a3a4a5a6a7a8a9"},
    {"orange", "o0o1o2o3o4o5o6o7o8o9"},
    {"deep-orange", "O0O1O2O3O4O5O6O7O8O9"},
    {"brown", "n0n1n2n3n4n5n6n7n8n9"},
    {"grey", "w0w1w2w3w4w5w6w7w8w9"},
    {"blue-grey", "W0W1W2W3W4W5W6W7W8W9"},
    {"rgb", "rgb"},
    {"rainbow", "rkpPibBctgGlyaoO"},
    {"cool", "pPibBctg"},
    {"hot", "yaoOrk"},
    {"RdYlGn", "ry3g"},
    {"RdYlBu", "ry3b"},
    {"RdGr", "rw"},
    {"RdBu", "rb"},
    {"PIYG", "kl"},
    {"PRGn", "Pg"},
    {"OrPu", "op"},
    {"YlGn", "y1g5g9"},
    {"YlGnBu", "y1g3b6b9"}
};

std::vector<uint32_t> Pallet(const std::string& pallet);
std::vector<uint32_t> RandPallet();

#endif  // TRIGON_PALLET_H_
