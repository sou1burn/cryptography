#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstdlib>

#ifndef BMP_h
#define Bmp_h

using byte = uint8_t;

#pragma pack(push, 1)
struct BmpFileHeader
{
    uint16_t file_type {0x4D42};
    uint32_t file_size {0};
    uint16_t reserved1 {0};
    uint16_t reserved2 {0};
    uint32_t offset {0};
};


struct BmpInfoHeader 
{
    uint32_t size {0};
    int32_t width {0};
    int32_t height {0};

    uint16_t planes {1};
    uint16_t bit_count {0};
    uint32_t compression {0};
    uint32_t image_size {0};
    int32_t x_pixels_per_meter {0};
    int32_t y_pixels_per_meter {0};
    uint32_t colors_used {0};
    uint32_t colors_important {0};
};

#pragma pack(pop)

class BmpReader
{
private:
    std::ifstream bmp_file;
    BmpFileHeader file_header;
    BmpInfoHeader info_header;
    std::vector<byte> pixel_data;

public:
    void read_data(const std::string& filename);

    void rewrite_bmp(const std::string& filename, std::vector<byte> data);

    const std::vector<byte>& get_pixel_data(){ return pixel_data; };

    size_t get_pixel_data_size() const { return pixel_data.size(); };

    const BmpFileHeader& get_file_header() { return file_header; };

    const BmpInfoHeader& get_info_header() { return info_header; };
};

#endif