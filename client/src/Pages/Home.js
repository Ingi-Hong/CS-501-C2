import { useState } from "react";
import "spectre.css";

function Home() {
  const [refresh, setRefresh] = useState("plee");

  return (
    <div className="d-flex">
      <div className="p-centered">
        <h1>Administration</h1>
      </div>
    </div>
  );
}

export default Home;