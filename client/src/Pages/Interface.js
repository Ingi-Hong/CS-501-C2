import { useState } from "react";
import CommandDisplay from "../Components/CommandDisplay/CommandDisplay";
import Console from "../Components/Console";
import ImplantTable from "../Components/ImplantTable";
import QueueCommandForm from "../Components/QueueCommandForm";

function Interface() {
  const [refresh, setRefresh] = useState("plee");
  const [ID, setID] = useState();
  return (
    <div className="container">
      <ImplantTable setID={setID}></ImplantTable>

      <div className="columns">
        <div className="column col-12 m-2 p-2">
          <Console id={ID} refresh={refresh} />
        </div>

        <div className="column col-12 m-2 p-2">
          <CommandDisplay />
        </div>
      </div>
    </div>
  );
}

export default Interface;
