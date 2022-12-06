import { Navigate, Outlet } from "react-router-dom";
import "spectre.css";
import AuthService from "./AuthService";
import NavBar from "./NavBar";
import "./Stylesheets/defaultpage.css";

function DefaultPage() {
  const isLoggedIn = AuthService.isLoggedIn();
  if (!isLoggedIn) {
    return <Navigate to="/login" />;
  } else {
    return (
      <div className="defaultPage-wrapper">
        <NavBar />
        <div className="outlet-wrapper">
        <Outlet />
        </div>
      </div>
    );
  }
}

export default DefaultPage;
