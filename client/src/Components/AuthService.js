import { bake_cookie, delete_cookie, read_cookie } from "sfcookies";

async function sendLogoutRequest() {
  try {
    let response = await fetch(process.env.REACT_APP_C2URL + "/logout", {
      method: "POST",
      mode: "cors",
      body: "",
      headers: {
        "Content-Type": "application/x-www-form-urlencoded",
        Authorization: `Bearer ${read_cookie("access_token")}`,
      },
    });
    let data = await response.json();
    AuthService.login(data.access_token);
    if (response.status === 200) {
      return "Logout succesful";
    } else {
      return "Error logging in";
    }
  } catch (error) {
    return error.message;
  }
}

class AuthService {
  static isLoggedIn() {
    let cookie = read_cookie("access_token");
    console.log(cookie);
    if (cookie.length === 0) {
      return false;
    } else {
      return true;
    }
  }
  static async logout() {
    let string = await sendLogoutRequest();
    delete_cookie("access_token");
    return string;
  }
  static login(value) {
    bake_cookie("access_token", value, Date.now() + 1800);
  }
}

export default AuthService;
