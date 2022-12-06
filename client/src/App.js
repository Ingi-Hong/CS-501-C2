import React from "react";
import { BrowserRouter, Navigate, Route, Routes } from "react-router-dom";
import "./App.css";
import DefaultPage from "./Components/DefaultPage";
import LoginPage from "./Components/LoginPage";
import Home from "./Components/Home";
import TestImplantFun from "./Components/TestImplantFun";

function App() {
  return (
    <div className="site-wrapper">
      <BrowserRouter>
        <Routes>
          <Route path="/login" element={<LoginPage />} />
          <Route path="/" element={<Navigate to="/login" />} />
          <Route element={<DefaultPage />}>
            <Route path="/home" element={<Home />} />
            <Route path="/test_implant" element={<TestImplantFun />} />
          </Route>
        </Routes>
      </BrowserRouter>
    </div>
  );
}

export default App;
