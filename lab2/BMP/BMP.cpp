#include "BMP.h"

namespace lab2
{

void BmpReader::read_data(const std::string& filename)
{
    bmp_file.open(filename, std::ios::binary);
    
    if (!bmp_file)
    {
        throw std::runtime_error("File not opened\n");
    }
    
    bmp_file.read((char*)(&file_header), sizeof(BmpFileHeader));

    if (file_header.file_type != 0x4D42)
    {
        throw std::runtime_error("File is not .bmp\n");
    }

    bmp_file.read((char*)(&info_header), sizeof(BmpInfoHeader));

    if (info_header.bit_count != 24)
    {
        throw std::runtime_error("Unsupported format\n");
    }


    size_t data_size = file_header.file_size - file_header.offset;

    pixel_data.resize(data_size);

    bmp_file.seekg(file_header.offset, std::ios::beg);

    bmp_file.read(reinterpret_cast<char*>(pixel_data.data()), data_size);

    bmp_file.close();

}


void BmpReader::rewrite_bmp(const std::string& filename, std::vector<byte> data)
{
    std::ofstream out(filename, std::ios::binary);

    if (!out)
    {
        throw std::runtime_error("Output file not opened");
    }

    out.write(reinterpret_cast<char*>(&file_header), sizeof(BmpFileHeader));
    out.write(reinterpret_cast<char*>(&info_header), sizeof(BmpInfoHeader));

    if(data.size() != pixel_data.size())
    {
        throw std::runtime_error("Size of pixel data != original pixel data size");
    }

    out.write(reinterpret_cast<char*>(data.data()), data.size());

    out.close();
}

void BmpReader::encrypt_bmp(const std::string &input, const std::string &output, size_t block_size, Key &key)
{

    read_data(input);

    FEAL_crypt encrypter(32, key);

    Block key_block;

    for (size_t i = 0; i < key.size(); ++i)
    {
        key_block.push_back(key[i]);
    }

    for (size_t i = 0; i < pixel_data.size(); i+=block_size)
    {
        Block block(pixel_data.begin() + i, pixel_data.begin() + std::min(i + block_size, pixel_data.size()));
        
        encrypter.encrypt(block);

        std::copy(block.begin(), block.end(), pixel_data.begin() + i);

    }

    rewrite_bmp(output, pixel_data);
}

void BmpReader::decrypt_bmp(const std::string &input, const std::string &output, size_t block_size, Key &key)
{

    read_data(input);

    FEAL_crypt decryptor(32, key);

    Block key_block;


    for (size_t i = 0; i < key.size(); ++i)
    {
        key_block.push_back(key[i]);
    }

    for (size_t i = 0; i < pixel_data.size(); i += block_size)
    {
        Block block(pixel_data.begin() + i, pixel_data.begin() + std::min(i + block_size, pixel_data.size()));
        
        decryptor.decrypt(block);

        std::copy(block.begin(), block.end(), pixel_data.begin() + i);
    }
    
    rewrite_bmp(output, pixel_data);
}


void BmpReader::encrypt_bmp_cbc(const std::string &input, const std::string &output, size_t block_size, Key &key, Block& iv, size_t corrupt_byte_idx)
{

    read_data(input);

    FEAL_crypt encrypter(32, key);
    
    Block key_block;

    for (size_t i = 0; i < key.size(); ++i)
    {
        key_block.push_back(key[i]);
    }

    for (size_t i = 0; i < pixel_data.size(); i+=block_size)
    {
        Block block(pixel_data.begin() + i, pixel_data.begin() + std::min(i + block_size, pixel_data.size()));
        
        encrypter.encrypt_cbc(block, iv);

        std::copy(block.begin(), block.end(), pixel_data.begin() + i);

    }
    if (corrupt_byte_idx < pixel_data.size()) 
    {
        pixel_data[corrupt_byte_idx] = ~pixel_data[corrupt_byte_idx];  
    }

    rewrite_bmp(output, pixel_data);
}

void BmpReader::corrupt(const std::string& file)
{
    bmp_file.close();
    bmp_file.open(file, std::ios::binary | std::ios::in | std::ios::out);
    
    if (!bmp_file)
    {
        throw std::runtime_error("File not opened\n");
    }
    
    bmp_file.read((char*)(&file_header), sizeof(BmpFileHeader));

    if (file_header.file_type != 0x4D42)
    {
        throw std::runtime_error("File is not .bmp\n");
    }

    bmp_file.read((char*)(&info_header), sizeof(BmpInfoHeader));

    if (info_header.bit_count != 24)
    {
        throw std::runtime_error("Unsupported format\n");
    }


    size_t data_size = file_header.file_size - file_header.offset;

    pixel_data.resize(data_size);
    size_t corrupt_pos = data_size - 1;
    bmp_file.seekg(file_header.offset, std::ios::beg);
    bmp_file.seekg(corrupt_pos);
    byte b;
    bmp_file.read((char*)&b, 1);
    b = ~b;
    bmp_file.seekg(corrupt_pos);
    //bmp_file.write((char *)&b, 1);
    bmp_file.close();
}

void BmpReader::decrypt_bmp_cbc(const std::string &input, const std::string &output, size_t block_size, Key &key, Block& iv, size_t corrupt_byte_idx)
{

    read_data(input);

    FEAL_crypt decryptor(32, key);

 /*   if (corrupt_byte_idx < pixel_data.size())
    {
        std::cout << "do" << static_cast<int>(pixel_data[corrupt_byte_idx]);
        pixel_data[corrupt_byte_idx] = ~pixel_data[corrupt_byte_idx];  
        std::cout << "posle" <<static_cast<int>(pixel_data[corrupt_byte_idx]);
        pixel_data[corrupt_byte_idx + 1] = ~pixel_data[corrupt_byte_idx];
    }
*/
    Block key_block;


    for (size_t i = 0; i < key.size(); ++i)
    {
        key_block.push_back(key[i]);
    }

    for (size_t i = 0; i < pixel_data.size(); i += block_size)
    {
        Block block(pixel_data.begin() + i, pixel_data.begin() + std::min(i + block_size, pixel_data.size()));
        
        decryptor.decrypt_cbc(block, iv);

        std::copy(block.begin(), block.end(), pixel_data.begin() + i);
    }
    
    rewrite_bmp(output, pixel_data);
}
} 