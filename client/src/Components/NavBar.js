import { Link, Navigate } from "react-router-dom";
import { useState } from "react";
import "spectre.css";
import AuthService from "./AuthService";

function NavigationBar(props) {

  var setLogin = props.setLogin;

  async function handleLogout() {
    await AuthService.logout();
    setLogin(false);
  }

    return (
      <header className="navbar">
        <section className="navbar-section">
          <Link className="btn btn-link" to="/home">
            Home
          </Link>

          

          <Link className="btn btn-link" to="/implant_table">
            Implant Table
          </Link>

          <Link className="btn btn-link" to="/interface">
            Interface
          </Link>

          
        </section>

        <section className="navbar-center">
          <img
            src={require("./Images/macroyal.jpg")}
            style={{ width: "100px" }}
          ></img>
        </section>

        <section className="navbar-section">
          <Link className="btn btn-link" to="/test_implant">
            Test Implant Functions
          </Link>
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
