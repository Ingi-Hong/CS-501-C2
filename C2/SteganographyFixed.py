import imageio as iio


# Example Usage:
#
# img = iio.imread("doge.png")
# iio.imwrite("doge_encoded.png", encode(img, "HelloWorld"))
# print(decode(iio.imread("doge_encoded.png")))


def decode_length(img):
    i = 0
    length = 0
    while i < 64:
        length <<= 1
        bit = img[0][i][2] & 1
        length ^= (-bit ^ length) & 1
        i += 1
    return length


def encode_length(length, img):
    i = 63
    j = 0
    while i >= 0:
        bit = (length >> j) & 1
        img[0][i][2] ^= (-bit ^ img[0][i][2]) & 1
        i -= 1
        j += 1


def decode_single_byte(img, i, j):
    to_decode = 0
    w = j
    count = 0
    for h in range(i, img.shape[0]):
        while w < img.shape[1] and count < 64:
            bit = img[h][w][2] & 1
            to_decode <<= 1
            to_decode ^= (-bit ^ to_decode) & 1
            count += 1
            w += 1
        w = 0
        h += 1
    return chr(to_decode)


def encode_single_byte(to_encode, img, i, j):
    w = i
    count = 63
    h = j
    for h in range(j, img.shape[0]):
        while w < img.shape[1]:
            bit = (ord(to_encode) >> count) & 1
            count -= 1
            img[h][w][2] ^= (-bit ^ img[h][w][2]) & 1
            w += 1
            if count == -1:
                return w, h
        w = 0
        h += 1
    return w, h


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
    while nr < total_pixels and nr < length:
        first = nr % img.shape[1]
        second = nr // img.shape[1]
        msg += decode_single_byte(img, second, first)
        nr += 64
    return msg


# img = iio.v2.imread("doge.png")
# iio.imwrite("doge_super_Cat03.png", encode(img, "asddasfsdfsdfdsf"))
# # print(decode(iio.v3.imread("encrypted_doge.png")))
# img = iio.v3.imread("doge_super_Cat03.png")
# print(decode(img))

def createEncodedImage(image: str, data: str, encodedImageName: str):
    try:
        writeThis = iio.v2.imread(image)
        iio.imwrite(encodedImageName, encode(writeThis, data)) 
        return None
    except Exception as e:
        return e 


def readEncodedImage(fileName: str):
    readThis = iio.v3.imread(fileName)
    return decode(readThis)

# createEncodedImage("doge.png", "poofdkajsdnflkajnsdkfljanskdljnflkajsdnfkljasndlkfjanslkdjfnlkasjdnflkajsdnlfkjasdnlkfjansdlkfjanslkdjfnlaksjdnflakjsdnflkjasnflkjsadnlkfjansdlkjfansldkjfnlskjdnflasjdnflajsdnfklajsdnflkjadnslkfjanslkdjfnalksjdnflkjasndflkjnasldkjfnlksjdnflkjasndlfkjnasldkjfnlaskjdnflaksjdnflkasjdnflkasjdnflkasjdnflkjsadnfkljasdnfp", "test.png")
# print(readEncodedImage("test.png"))