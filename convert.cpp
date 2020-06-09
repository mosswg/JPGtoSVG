#include <iostream>
#include <fstream>
#include <string>
#include "base64/base64.h"
#include "base64/base64.cpp"

int main()
{
    bool url;
    std::string image;
    std::string encoded;
    std::string alldata = "";
    std::cout << "Name of file without extension: \n";
    std::cin >> image;
    // Takes the name of the image
    std::string outfile = image + ".svg";
    // creates outfiles
    image = image + ".jpg";
    // adds jpg extension to name of file
    std::string line;
    std::ifstream input(image, std::ios::in | std::ios::binary);
    // opens the image

    if (input.is_open()) {
    char ch;
    while (input.get(ch)) {
         alldata += ch;
    }
    input.close();
}
    else {
        std::cout << "ERROR: File not found";
    }

    encoded = base64_encode(reinterpret_cast<const unsigned char*>(alldata.c_str()), alldata.length(), url = false);
    // encodes the one line of image data
    std::string svg;
    std::ofstream output(outfile,std::ios::binary|std::ios::out);
            svg = "<?xml version='1.0' encoding='UTF-8' standalone='no'?> <!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' 'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'> <svg version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' x='0px' y='0px' width='1920px' height='1200px' viewBox='0 0 1920 1200' enable-background='new 0 0 1920 1200' xml:space='preserve'>  <image id='image0' width='1920' height='1200' x='0' y='0' href='data:image/png;base64,";
            svg = svg + encoded;
            svg = svg + "'/> </svg>";
            output << svg;







}



