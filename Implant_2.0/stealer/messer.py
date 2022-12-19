import json

def newParseJSON(json_res):
    key = json_res['KEY']
    cookies = json_res['COOKIES']
    passwords = json_res['PASSWORDS']
    username_list = []
    password_list = []
    url_list = []
    host_key_list = []
    encrypted_value_list = []
    prof_pw_list = []
    prof_cook_list = []
    for user in passwords:
        for entry in passwords[user]:
            password_list.append(decrypt_password(entry['password'], key))
            username_list.append(entry['username'])
            url_list.append(entry['url'])
            prof_pw_list.append(user)
    
    #print(password_list)
    for user in cookies:
        for entry in cookies[user]:
            host_key_list.append(entry['HOST_KEY'])
            encrypted_value_list.append(decrypt_password(entry['encrypted value'], key))
            prof_cook_list.append(user)

    cookie_table = [host_key_list, encrypted_value_list]
    password_table = [username_list, password_list, url_list]
    return cookie_table, password_table
            

def main():
    f = open('../newbad.txt')
    g = f.read()
    c = json.loads(g)
    cookie_table, password_table = newParseJSON(c)
    



    

#50-58
main()