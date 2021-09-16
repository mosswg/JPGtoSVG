#include <string>
#include <iostream>
#include <fstream>
#include <base64.h>

struct jpeg_data {
int bits_per_channel;
int height;
int width;
int number_color_components;
};

const unsigned char EMPTY = 0;
const unsigned char SOI = 0xd8; // Start of Image
const unsigned char EOI = 0xd9; // End of Image
const unsigned char TEM = 0x01;
const unsigned char SOF0 = 0xc0; // Start of Frame
const unsigned char reset_marker_min = 0xd0;
const unsigned char reset_marker_max = 0xd7;


struct jpeg_data get_jpeg_data(std::string data) {          // data is an array of bytes
    int off = 0;
    unsigned char marker;
    while(off < data.length()) {
        while(((unsigned char) data[off]) == 255) off++;
        marker = data[off];
        off++;

        if(marker == SOI) continue;
        if(marker == EOI) break;
        if(reset_marker_min <= marker && marker <= reset_marker_max) continue;
        if(marker == TEM) continue;

        int len = (data[off]<<8) | data[off+1];  off+=2;

        if(marker == SOF0) {
            return {
                    .bits_per_channel = data[off],
                    .height = (((unsigned char)data[off+1])<<8) | ((unsigned char)data[off+2]),
                    .width = (((unsigned char)data[off+3])<<8) | (unsigned char)data[off+4],
                    .number_color_components = data[off+5]
            };
        }
        off+=len-2;
    }

    return {};
}


bool has_ending (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}



int main(int argc, char *argv[])
{
    bool url;
    std::string in_file_name = "test.jpg";
    std::string out_file_name;
    std::string encoded_data;
    std::string all_data;
    int arg_width = 0;
    int arg_height = 0;
    std::string arg_out_file_name;
    if(argc<2){
        std::cout << "Name of file: ";
        getline(std::cin, in_file_name);
    }
    else {
        if (argc > 2) {
            for (int i = 1; i < argc-1; i++) {
                std::string arg(argv[i]);
                if (arg[0] == '-') {
                    switch (arg[1]) {
                        case 'w':
                            arg_width = std::stoi(std::string(argv[++i]));
                            break;
                        case 'h':
                            arg_height = std::stoi(std::string(argv[++i]));
                            break;
                        case 'o':
                            arg_out_file_name = std::string(argv[++i]);
                            break;
                    }
                }
            }
        }
        in_file_name = argv[argc - 1];
    }
    
    if (in_file_name.find('.') != std::string::npos) {
        out_file_name = in_file_name.substr(0, in_file_name.find_first_of('.'));
    }
    else {
        // adds jpg extention to name of file if it doesn't already have one.
        out_file_name = in_file_name;
        in_file_name.append(".jpg");
    }

    if (arg_out_file_name.empty()) {
        out_file_name.append(".svg");
    }
    else {
        out_file_name = arg_out_file_name;
    }


    std::string line;
    std::ifstream image_file(in_file_name.c_str(), std::ios::in | std::ios::binary);
    // opens the image

    if (image_file.is_open()) {
        char ch;
        while (image_file.get(ch)) {
            all_data += ch;
        }
        image_file.close();
    }
    else {
        std::cout << "ERROR: File not found";
    }

    struct jpeg_data image_data{};
    if (!(arg_width && arg_height)) {
        image_data = get_jpeg_data(all_data);
        image_data.width = (((arg_width == 0) * image_data.width) + ((arg_width != 0) * arg_width));
        image_data.width = (((arg_height == 0) * image_data.height) + ((arg_height != 0) * arg_height));
    }
    else {
        image_data.height = arg_height;
        image_data.width = arg_width;
    }

    encoded_data = base64_encode(reinterpret_cast<const unsigned char*>(all_data.c_str()), all_data.length(), url = false);
    // encodes the one line of image data
    std::string svg;
    std::ofstream out_file(out_file_name, std::ios::binary | std::ios::out);
    svg = "<?xml version='1.0' encoding='UTF-8' standalone='no'?> \n<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' 'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'> \n<svg version='1.0' xmlns='http://www.w3.org/2000/svg' width='" + std::to_string(image_data.width) + "px' height='" + std::to_string(image_data.height) + "px' viewBox='0 0 " + std::to_string(image_data.width) + " " + std::to_string(image_data.height) + "' preserveAspectRatio='xMidYMid meet'>  \n<image id='image0' width='" + std::to_string(image_data.width) + "' height='" + std::to_string(image_data.height) + "' x='0' y='0' href='data:image/jpeg;base64,";
    svg = svg + encoded_data;
    svg = svg + "'/> \n</svg>";
    out_file << svg;

    std::cout << "Created file " << out_file_name << " with a width of " << image_data.width << " and a height of " << image_data.height << std::endl;


}



