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
      <div className="columns">
        <div className="column col-12">
          <ImplantTable setID={setID}></ImplantTable>
        </div>

        <div className="column col-12">
          <Console className="column col-12" id={ID} refresh={refresh} />
        </div>

        <div className="column col-12">
          <CommandDisplay id={ID}/>
        </div>
      </div>
    </div>
  );
}

export default Interface;
