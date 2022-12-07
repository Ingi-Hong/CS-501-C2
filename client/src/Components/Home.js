import QueueCommandForm from "./QueueCommandForm";
import CommandDisplay from "./CommandDisplay";
import "spectre.css";

function Home() {
  return (
    <div className="container">
        <h2>Execute</h2>
          <QueueCommandForm />
          <CommandDisplay />
      </div>
  );
}

export default Home;
