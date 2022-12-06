import React from "react";
import {
  BrowserRouter, Route,
  Routes
} from "react-router-dom";
import "./App.css";
import DefaultPage from "./Components/DefaultPage";
import Home from "./Components/Home";
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
