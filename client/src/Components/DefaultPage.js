import { Navigate, Outlet } from "react-router-dom";
import "spectre.css";
import AuthService from "./AuthService";
import NavBar from "./NavBar";

function DefaultPage() {
  const isLoggedIn = AuthService.isLoggedIn();
  if (!isLoggedIn) {
    return <Navigate to="/login" />;
  } else {
    return (
      <div>
        <NavBar />
        <Outlet />
      </div>
    );
  }
}

export default DefaultPage;
