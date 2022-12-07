import QueueCommandForm from "./QueueCommandForm";
import CommandDisplay from "./CommandDisplay";
import "spectre.css";

function Home() {
  return (
    <div className="p-centered">
      <div className="m-2 p-2">
        <h2>Execute</h2>
          <QueueCommandForm />
      </div>
      <div className="m-2 p-2">
          <h2>Command Display</h2>

          <CommandDisplay />
      </div>
      </div>
  );
}

export default Home;
