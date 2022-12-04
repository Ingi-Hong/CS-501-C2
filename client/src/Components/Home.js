import { Navigate } from "react-router-dom";
import AuthService from "./AuthService";
import QueueCommandForm from "./QueueCommandForm";

function Home() {
    
    return (
      <div className="home-content-container">
        hi!
        <QueueCommandForm />
      </div>
    );
}

export default Home;
