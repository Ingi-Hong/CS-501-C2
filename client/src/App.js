import React from "react";
import "./App.css";
import LoginForm from "./Components/LoginForm";
import {
  Router,
  Route,
  Routes,
  Outlet,
  BrowserRouter,
  Navigate,
} from "react-router-dom";
import Home from "./Components/Home";
import DefaultPage from "./Components/DefaultPage";
import LoginPage from "./Components/LoginPage";

function App() {
  return (
    <div className="site-wrapper">
      <BrowserRouter>
        <Routes>
          <Route element={<DefaultPage />}>
            <Route path="/" element={<Home />} />
          </Route>
          <Route path="/login" element={<LoginPage />} />
        </Routes>
      </BrowserRouter>
    </div>
  );
}

export default App;
