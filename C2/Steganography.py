import imageio as iio


def encode(img, msg):
    """Encode the message into the image.
    :param img: Image to encode the message into.
    :param msg: Message to encode.
    :return: Image with the message encoded.
    """
    i = 63
    j = 0
    length = len(msg)
    while i >= 0:
        bit = (length >> j) & 1
        img[i][j][2] ^= -bit ^ (img[i][j][2] & 1)
        i -= 1
        j += 1
    i = 64
    j = 0
    for c in msg:
        i, j = encode_single_byte(c, img, i, j)
    return img


def encode_single_byte(to_encode, img, i, j):
    for c in bin(ord(to_encode))[2:].zfill(8):
        img[i][j][2] ^= -int(c) ^ (img[i][j][2] & 1)
        j += 1
        if j >= img.shape[1]:
            i += 1
            j = 0
    return i, j


def decode(img):
    """Decode the message from the image.
    :param img: Image to decode the message from.
    :return: Decoded message.
    """
    i = 63
    j = 0
    length = 0
    while i >= 0:
        length |= (img[i][j][2] & 1) << (63 - i)
        i -= 1
        j += 1
    i = 64
    j = 0
    msg = ''
    while len(msg) < length:
        c, j = decode_single_byte(img, i, j)
        msg += c
        if j >= img.shape[1]:
            i += 1
            j = 0
    return msg


def decode_single_byte(img, i, j):
    byte = ''
    while len(byte) < 8:
        byte += str(img[i][j][2] & 1)
        j += 1
    return chr(int(byte, 2)), j


img = iio.imread("doge.png")
iio.imwrite("doge_encoded.png", encode(img, "HelloWorld"))
print(decode(iio.imread("doge_encoded.png")))
