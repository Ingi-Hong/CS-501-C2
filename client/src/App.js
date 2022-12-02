import React from "react";
import "./App.css";
import LoginForm from "./Components/LoginForm";
import { Route, BrowserRouter } from "react-router-dom";
import Cookies from 'js-cookie';
import Home from "./Components/Home";
import NavBar from "./Components/NavBar";

const DefaultPage = () => (
  <div>
    <NavBar />
    <BrowserRouter>
      <Route path="/main" element={<Home />}/>
    </BrowserRouter>
  </div>
);

function App() {

  return (
    <React.StrictMode>
      <BrowserRouter>
        <Route element={<DefaultPage />} />
        <Route path="/login" element={<LoginForm />} />
      </BrowserRouter>
    </React.StrictMode>
  );
}

export default App;
