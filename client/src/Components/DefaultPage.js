import { useEffect, useState } from "react";
import { Navigate, Outlet } from "react-router-dom";
import "spectre.css";
import AuthService from "./AuthService";
import NavBar from "./NavBar";

function DefaultPage() {
  const [isLoggedIn, setIsLoggedIn] = useState();

  function checkState() {
    const someBool = AuthService.isLoggedIn();
    setIsLoggedIn(someBool);
  }

  useEffect(() => {
    checkState();
  }, []);
  
  console.log("is logged in:" + isLoggedIn);
  if (isLoggedIn === false) {
    console.log(!isLoggedIn);
    return <Navigate to="/login" />;
  }
  return (
    <div>
      <NavBar />
      <Outlet />
    </div>
  );
}

export default DefaultPage;
