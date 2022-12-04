import { useState } from "react";
import AuthService from "./AuthService";
function NavBar(){

    const [loggedIn, setLoggedIn] = useState(AuthService.isLoggedIn());

    async function handleLogout(){
        await AuthService.logout();
        setLoggedIn = true;
    }
    
    return (
        <div className="navbar-wrapper">
            <button onClick={async () => await handleLogout()}>Logout</button>
        </div>

    );
}

export default NavBar;
