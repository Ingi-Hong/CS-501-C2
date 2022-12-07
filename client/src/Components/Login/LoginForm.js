import React, { useState } from "react";
import { Navigate } from "react-router-dom";
import AuthService from "../AuthService";
import "../Stylesheets/loginform.css";

function LoginForm() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState("");
  const [message, setMessage] = useState("");

  async function handleSubmit() {
    setIsLoading(true);
    try {
      let response = await fetch(process.env.REACT_APP_C2URL + "/login", {
        method: "POST",
        mode: "cors",
        body: new URLSearchParams({
          username: username,
          password: password,
        }),
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
      });
      let data = await response.json();
      console.log("access_code: " + Object.keys(data));
      AuthService.login(data.access_token);
      if (response.status === 200) {
        setMessage("Login succesful");
      } else if (response.status === 401) {
        setMessage("Wrong username or password");
      } else {
        setMessage("Error logging in");
      }
    } catch (error) {
      setError(error.message);
    } finally {
      setIsLoading(false);
    }
  }

  const onClick = async (e) => {
    e.preventDefault();
    await handleSubmit();
  };

  
  if (error) return <div>error: {error}</div>;
  if (message === "Login succesful") return <Navigate to="/home" />;
  return (
    <div className="form-wrapper">
      <div> {message} &nbsp; {isLoading}</div>
      <form
        onSubmit={async (e) => {
          onClick(e);
        }}
        className="form-group"
      >
        <label className="form-label" for="username">
          Username: &nbsp;
          <input
          id="username"
            name="username"
            type="text"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
          />
        </label>
        <br />
        <label className="form-label" for="password">
          Password: &nbsp;
          <input
          id="password"
            name="password"
            type="text"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
        </label>
        <button className="btn " type="submit">Login</button>
      </form>
    </div>
  );
}

export default LoginForm;
