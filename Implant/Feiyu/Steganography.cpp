#include <string>
#include <assert.h>
#include <iostream>
#include <functional>

#define cimg_use_png
#include "CImg.h"

using namespace cimg_library;
using namespace std;

//https://github.com/jokLiu/ImageSteganography https://www.codeproject.com/Articles/4877/Steganography-Hiding-messages-in-the-Noise-of-a-Pi
	void enocde_length(const uint64_t length,
		CImg<unsigned char>& img) {
		int bit;
		for (int i = 63, j = 0; i >= 0; i--, j++) {
			bit = (length >> j) & 1;
			img(i, 0, 2) ^= (-bit ^ img(i, 0, 2)) & 1;
		}
	}

	uint64_t decode_length(CImg<unsigned char>& img) {
		uint64_t length = 0;
		int bit;
		for (int i = 0; i < 64; i++) {
			length <<= 1;
			bit = img(i, 0, 2) & 1;
			length ^= (-bit ^ length) & 1;
		}
		return length;
	}

	std::pair<int, int> encode_single_byte(uint8_t to_encode,
		CImg<unsigned char>& img,
		std::pair<int, int>& coord) {
		int shift_count = 64 - 1;
		int bit;
		int w = coord.first;
		for (int h = coord.second; h < img.height(); h++) {
			for (; w < img.width(); w++) {
				bit = (to_encode >> shift_count) & 1U;
				shift_count--;
				img(w, h, 2) ^= (-bit ^ img(w, h, 2)) & 1U;

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

	

	


	char decode_single_byte(CImg<unsigned char>& img,
		std::pair<int, int>& coord) {
		int bit = 0;
		uint8_t to_decode = 0;
		int w = coord.first;
		int count = 0;
		for (int h = coord.second; h < img.height(); h++) {
			for (; w < img.width() && count < 64; w++, count++) {
				bit = img(w, h, 2) & 1U;
				to_decode <<= 1;
				to_decode ^= (-bit ^ to_decode) & 1U;
			}
			w = 0;
		}
		cout << (int)to_decode;
		return to_decode;
	}
public:
	std::string decode(const std::string& img_name) {
		CImg<unsigned char> img(img_name.c_str());
		std::string message = "";
		uint64_t length = 0;
		length = decode_length(img) * 64
			+ 64;
		int width = img.width();
		int total_pixels = width * img.height();
		auto coord = std::make_pair(64, 0);
		for (int nr = 64; nr < total_pixels && nr < length;
			nr += 64) {
			coord.first = nr % width; // width
			coord.second = nr / width; //height
			message += decode_single_byte(img, coord);
		}
		return message;
	}

	void encode(std::string& img_name,
		std::string& message) {
		CImg<unsigned char> img(img_name.c_str());
		uint64_t length = message.length();
		enocde_length(length, img);
		auto coord = std::make_pair(64, 0);
		for (char c : message) {
			coord = encode_single_byte(c, img, coord);
		}
		std::string new_img_name = "encrypted_" + img_name;
		img.save(new_img_name.c_str());
	}

int main() {
	Steganography encrypt;
	//The message you want to encode
	std::string str = "a";
	//path of the picture
	std::string pic = "doge.png";
	//encode function
	encrypt.encode(pic, str);
	//decode and cout, modify this cout to send html message so server can recive this 
	cout << encrypt.decode("encrypted_doge.png") << "\n";

}
