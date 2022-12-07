import QueueCommandForm from "./QueueCommandForm";
import CommandDisplay from "./CommandDisplay";
import "spectre.css";

function Home() {
  return (
    <div className="container">
      <div className="columns">
        <div className="column col-3 m-2 p-2 p-centered">
          <h2>Execute</h2>
          <QueueCommandForm />
        </div>
      </div>
      <div className="columns">
        <div className="column col-12 m-2 p-2">
          <h2>Command Display</h2>

          <CommandDisplay />
        </div>
      </div>
    </div>
  );
}

export default Home;
