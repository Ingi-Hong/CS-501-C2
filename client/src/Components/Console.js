import { useEffect, useRef, useState } from "react";

// https://bobbyhadz.com/blog/javascript-date-remove-local-timezone
function padTo2Digits(num) {
  return num.toString().padStart(2, '0');
}


function formatDate(date) {
  return (
    [
      date.getFullYear(),
      padTo2Digits(date.getMonth() + 1),
      padTo2Digits(date.getDate()),
    ].join('-') +
    ' ' +
    [
      padTo2Digits(date.getHours()),
      padTo2Digits(date.getMinutes()),
      padTo2Digits(date.getSeconds()),
    ].join(':')
  );
}

function Console(props) {
  const [message, setMessage] = useState();
  const [error, setError] = useState();

  let id = props.id;
  const idRef = useRef();
  const [history, setHistory] = useState();

  async function getCommandData(id_value) {
    var command_data = "No Commands Found";
    try {
      let response = await fetch(process.env.REACT_APP_C2URL + "/get_history", {
        method: "POST",
        mode: "cors",
        body: JSON.stringify({
          id: id_value,
        }),
        headers: {
          "Content-Type": "application/json",
        },
      });
      let respJSON = await response.json();
      if (typeof respJSON !== "undefined") {
        command_data = respJSON;
      }
      console.log(command_data);
      if (response.status === 200) {
      } else {
        setMessage("Error getting command data");
      }
    } catch (error) {
      setError("Error at getCommandData: " + error.message);
    } finally {
      return command_data;
    }
  }

  async function organizeHistory(id_value) {
    try {
      let command_data = await getCommandData(id_value);
      console.log(command_data);
      setHistory(command_data);
    } catch (error) {
      console.log("Error with console!: " + error);
    }
  }

  useEffect(() => {
    organizeHistory(id);
  }, [id]);

  return (
    <div className="container">
      <div className="columns">
        <div className="col col-12 centered">
          <div className="d-flex">
            <h1 className="p-centered">Console: {id}</h1>
          </div>
        </div>
        <div className="col col-12">
          <div className="float-left">
            {history && <div>{history.pending}</div>}
          </div>
        </div>
      </div>
    </div>
  );
}

export default Console;
