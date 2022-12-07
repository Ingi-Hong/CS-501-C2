import QueueCommandForm from "./QueueCommandForm";
import CommandDisplay from "./CommandDisplay";
import "spectre.css";

function Home() {
  return (
    <div className="container">
        <h2>Execute</h2>
          <QueueCommandForm />

          <h2>Command Display</h2>
          <CommandDisplay />
      </div>
  );
}

export default Home;
