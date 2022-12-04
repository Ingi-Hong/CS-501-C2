import "./Stylesheets/defaultpage.css";
import AuthService from "./AuthService";
import { Navigate, Outlet } from "react-router-dom";
import NavBar from "./NavBar";

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