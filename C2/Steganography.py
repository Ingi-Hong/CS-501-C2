import imageio as iio


# Example Usage:
#
# img = iio.imread("doge.png")
# iio.imwrite("doge_encoded.png", encode(img, "HelloWorld"))
# print(decode(iio.imread("doge_encoded.png")))

def encode_length(length, img):
    i = 63
    j = 0
    while i >= 0:
        bit = (length >> j) & 1
        img[i][0][1] ^= (-bit ^ img[i][0][1]) & 1
        i -= 1
        j += 1


def decode_length(img):
    i = 63
    j = 0
    length = 0
    while i >= 0:
        length |= (img[i][0][1] & 1) << (63 - i)
        i -= 1
        j += 1
    i = 64
    j = 0
    return length


def decode_single_byte(img, i, j):
    to_decode = 0
    w = i
    count = 0
    for h in range(j, img.shape[1]):
        while w < img.shape[0] and count < 64:
            bit = img[w][h][1] & 1
            to_decode <<= 1
            to_decode ^= (-bit ^ to_decode) & 1
            count += 1
            w += 1
        w = 0
        h += 1
    return chr(to_decode)


def encode_single_byte(to_encode, img, i, j):
    h = j
    w = i
    shift_count = 63
    to_decode = 0
    for h in range(j, img.shape[1]):
        while w < img.shape[0]:
            bit = (ord(to_encode[0]) >> shift_count) & 1
            shift_count -= 1
            img[w][h][1] ^= (-bit ^ img[w][h][1]) & 1
            if shift_count < 0:
                return w + 1, h
            w += 1
        w = 0
        h += 1
    return i, j


def encode(img, msg):
    """Encode the message into the image.
    :param img: Image to encode the message into.
    :param msg: Message to encode.
    :return: Image with the message encoded.
    """
    encode_length(len(msg), img)
    i = 64
    j = 0
    for c in msg:
        i, j = encode_single_byte(c, img, i, j)
    return img


def decode(img):
    """Decode the message from the image.
    :param img: Image to decode the message from.
    :return: Decoded message.
    """
    msg = ''
    length = decode_length(img) * 64 + 64
    total_pixels = img.shape[0] * img.shape[1]
    nr = 64
    first = 64
    second = 0
    while nr < total_pixels and nr < length:
        first = nr % img.shape[0]
        second = nr // img.shape[0]
        msg += decode_single_byte(img, first, second)
        nr += 64
    return msg
  
