symkey=None 

def xorcrypt(text,symkey):
    for x in range(len(text)):
        text[x]=text[x]^symkey[x%32]
    return text