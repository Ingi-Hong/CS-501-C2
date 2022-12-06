import { useState } from "react";
import AuthService from "./AuthService";
import "spectre.css";

function NavBar() {
  const [loggedIn, setLoggedIn] = useState(AuthService.isLoggedIn());

  async function handleLogout() {
    await AuthService.logout();
    setLoggedIn = true;
  }

  return (
    <header className="navbar">
      <section class="navbar-section">
        <button className="btn btn-link" onClick={async () => await handleLogout()}>Logout</button>
        <button className="btn btn-link" to="/test_implant">Test Implant Function</button>
      </section>
    </header>
  );
}

export default NavBar;
