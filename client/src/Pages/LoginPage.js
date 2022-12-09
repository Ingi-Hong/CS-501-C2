import { useEffect, useState } from "react";
import { Navigate } from "react-router-dom";
import "spectre.css";
import AuthService from "../Components/AuthService";
import LoginForm from "../Components/Login/LoginForm";
import MovingImage from "../Components/Login/movingimage";
import "../Components/Stylesheets/loginpage.css";

function LoginPage() {
  const [isLoggedIn, setIsLoggedIn] = useState();

  function checkState() {
    const someBool = AuthService.isLoggedIn();
    setIsLoggedIn(someBool);
  }
  
  useEffect(() => {
    checkState();
  }, []);

  if (isLoggedIn){
    return <Navigate to="/implant_table" />
  }
   else {
    return (
      <div className="page-wrapper">
        <div className="form-wrapper">
          <div className="hero-sm bg-purple">
            <div className="hero-body">
              <h1>MAC-WARE ZONE</h1>
                <LoginForm />
            </div>
          </div>
        </div>
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
        <MovingImage />
      </div>
    );
  }
}

export default LoginPage;
