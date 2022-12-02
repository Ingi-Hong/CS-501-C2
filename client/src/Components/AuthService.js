import React from "react";
import { bake_cookie, read_cookie, delete_cookie } from "sfcookies";

class AuthService {

    static isLoggedIn(){
        let cookie = read_cookie('access_token');
        console.log(cookie);
        if (cookie.length === 0){
            return false;
        } else {
            return true;
        }
    }

    static logout(){
        delete_cookie('access_token');
    }

    static login(value){
        bake_cookie('access_token', value, Date.now()+1800)
    }
};

export default AuthService;