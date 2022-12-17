import React from "react";
import { BrowserRouter, Navigate, Route, Routes } from "react-router-dom";
import "./App.css";
import DefaultPage from "./Pages/DefaultPage";
import LoginPage from "./Pages/LoginPage";
import Home from "./Pages/Home";
import TestImplantFun from "./Components/TestImplantFun";
import Interface from "./Pages/Interface";

function App() {
  return (
      <BrowserRouter>
        <Routes>
          <Route path="/login" element={<LoginPage />} />
          <Route path="/" element={<Navigate to="/login" />} />
          <Route element={<DefaultPage />}>
            <Route path="/home" element={<Home />} />
            <Route path="/test_implant" element={<TestImplantFun />} />
            <Route path="/interface" element={<Interface />} />
          </Route>
        </Routes>
      </BrowserRouter>
  );
}

export default App;
