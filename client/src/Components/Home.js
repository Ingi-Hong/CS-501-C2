import QueueCommandForm from "./QueueCommandForm";
import "spectre.css";

function Home() {
  return (
    <div className="container">
      <div className="columns">
        <div className="column col-12">
          <QueueCommandForm />
        </div>
        
      </div>
    </div>
  );
}

export default Home;
