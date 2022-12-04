import AuthService from "./AuthService";
import { Navigate } from "react-router-dom";
import LoginForm from "./LoginForm";
import "./Stylesheets/loginpage.css";
import MovingImage from "./movingimage";

function LoginPage() {
  const isLoggedIn = AuthService.isLoggedIn();
  if (isLoggedIn) {
    return <Navigate to="/" />;
  } else {
    return (
      
      <div className="page-wrapper">
        <MovingImage />
        <LoginForm />
      </div>
    );
  }
}

export default LoginPage;
