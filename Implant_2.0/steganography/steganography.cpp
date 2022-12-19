#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <iostream>

using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

//load png image
int loadPNG(const WCHAR* filename, Bitmap** bitmap) {
    *bitmap = Bitmap::FromFile(filename);
    if (*bitmap == NULL) {
        return 1;
    }
    return 0;
}

//encode length of message into first 64 pixels
void enocde_length(const uint64_t length, Bitmap* img) {
    int bit;
    for (int i = 63, j = 0; i >= 0; i--, j++) {
        Color color;
        bit = (length >> j) & 1;
        img->GetPixel(i, 0, &color);
        int r = color.GetR();
        int g = color.GetG();
        int b = color.GetB();
        //encode bit into LSB of blue channel
        b ^= (-bit ^ b) & 1U;
        Color new_color(r, g, b);
        img->SetPixel(i, 0, new_color);
    }
}

//encode single byte into image
std::pair<int, int> encode_single_byte(uint8_t to_encode, Bitmap* img, std::pair<int, int>& coord) {
    int bit;
    int shift_count = 64 - 1;
    int w = coord.first;
    //iterate through image
    for (int h = coord.second; h < img->GetHeight(); h++) {
        for (; w < img->GetWidth(); w++) {
            Color color;
            //get bit from byte
            bit = (to_encode >> shift_count) & 1U;
            shift_count--;
            img->GetPixel(w, h, &color);
            int r = color.GetR();
            int g = color.GetG();
            int b = color.GetB();
            //encode bit into LSB of blue channel
            b ^= (-bit ^ b) & 1U;
            Color new_color(r, g, b);
            //set pixel to new color
            img->SetPixel(w, h, new_color);
            if (shift_count < 0) {
                coord.first = w + 1;
                coord.second = h;
                goto end;
            }
        }
        w = 0;
    }
end:
    return coord;
}

//encode message into image
void encode(WCHAR* img_name, std::string& message, WCHAR* encrypt_name) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    Bitmap* img;
    //load image
    loadPNG(img_name, &img);
    uint64_t length = message.length();
    enocde_length(length, img);
    auto coord = std::make_pair(64, 0);
    //encode message into image
    for (char c : message) {
        coord = encode_single_byte(c, img, coord);
    }
    CLSID pngClsid;
    //save image, set the format to png
    CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
    img->Save(encrypt_name, &pngClsid, NULL);
    GdiplusShutdown(gdiplusToken);
}

//decode length of message from first 64 pixels
uint64_t decode_length(Bitmap* img) {
    uint64_t length = 0;
    int bit;
    for (int i = 0; i < 64; i++){
        length <<= 1;
        Color color;
        img->GetPixel(i, 0, &color);
        int b = color.GetB();
        bit = b & 1U;
        length ^= (-bit ^ length) & 1;
    }
    return length;
}

//decode single byte from image
char decode_single_byte(Bitmap* img, std::pair<int, int>& coord) {
    int bit = 0;
    int w = coord.first;
    int count = 0;
    uint8_t c = 0;

    for (int h = coord.second; h < img->GetHeight(); h++) {
        for (; w < img->GetWidth() && count < 64; w++, count++) {
            Color color;
            img->GetPixel(w, h, &color);
            int b = color.GetB();
            bit = b & 1U;
            c <<= 1;
            c ^= (-bit ^ c) & 1U;
        }
        w = 0;
    }
    return c;
}

//decode message from image
std::string decode(WCHAR* filepath) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    Bitmap* img;
    loadPNG(filepath, &img);
    uint64_t length = 0;
    length = decode_length(img) * 64 + 64;
    std::string message;
    int width = img->GetWidth();
    int total_pixels = width * img->GetHeight();
    auto coord = std::make_pair(64, 0);
    for (int i = 64; i < total_pixels && i < length; i += 64) {
        coord.first = i % width;
        coord.second = i / width;
        message += decode_single_byte(img, coord);
    }
    GdiplusShutdown(gdiplusToken);
    return message;
}

//main function for testing and debugging
int main() {
    WCHAR filepath[] = L"C:\\Users\\53444\\Downloads\\doge.png";
    WCHAR encryptpath[] = L"C:\\Users\\53444\\Downloads\\encrypted_doge.png";
    std::string message = "test message that is very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very long";
    encode(filepath, message, encryptpath);
    std::cout << decode(encryptpath) << std::endl;
    return 0;
}
