import React from "react";
import { BrowserRouter, Navigate, Route, Routes } from "react-router-dom";
import "./App.css";
import DefaultPage from "./Components/DefaultPage";
import LoginPage from "./Components/Login/LoginPage";
import Home from "./Components/Home";
import TestImplantFun from "./Components/TestImplantFun";
import ImplantTable from "./Components/ImplantTable";

function App() {
  return (
      <BrowserRouter>
        <Routes>
          <Route path="/login" element={<LoginPage />} />
          <Route path="/" element={<Navigate to="/login" />} />
          <Route element={<DefaultPage />}>
            <Route path="/home" element={<Home />} />
            <Route path="/test_implant" element={<TestImplantFun />} />
            <Route path="/implant_table" element={<ImplantTable />} />
          </Route>
        </Routes>
      </BrowserRouter>
  );
}

export default App;
