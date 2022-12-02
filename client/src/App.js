import React from "react";
import { redirect } from "react-router-dom";
import "./App.css";
import LoginForm from "./Components/LoginForm";
import { Router, Route, Routes, Outlet, BrowserRouter } from "react-router-dom";
import { bake_cookie, read_cookie, delete_cookie } from "sfcookies";
import Home from "./Components/Home";
import NavBar from "./Components/NavBar";
import AuthService from "./Components/AuthService";

const DefaultPage = () => {

  const isLoggedIn = AuthService.isLoggedIn();
  console.log(isLoggedIn)
  if (!isLoggedIn){
    return redirect("/login");
  }
  return (
    <div>
      <NavBar />
      <Outlet />
    </div>
  );
};

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route element={<DefaultPage />}>
          <Route path="/" element={<Home />} />
        </Route>
        <Route path="/login" element={<LoginForm />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;
