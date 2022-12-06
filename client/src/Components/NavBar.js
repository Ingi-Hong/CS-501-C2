import { Navigate } from "react";
import { Link } from "react-router-dom";
import AuthService from "./AuthService";
import "spectre.css";

function NavigationBar() {
  async function handleLogout() {
    await AuthService.logout();
  }

  if (!AuthService.isLoggedIn()) {
    window.location.reload(false);
    return <Navigate to="/login" />;
  }

  return (
    <header className="navbar">
      <section className="navbar-section">
        <Link className="btn btn-link" to="/test_implant">
          Test Implant
        </Link>

        <Link className="btn btn-link" to="/home">
          Home
        </Link>
      </section>
      
      <section class="navbar-center">
        <img src={require("./Images/macroyal.jpg")} style={{width: "50px"}}></img>
      </section>

      <section className="navbar-section">
        <button
          className="btn btn-link"
          onClick={async () => await handleLogout()}
        >
          Logout
        </button>
      </section>
    </header>
  );
}

export default NavigationBar;
