import { useEffect, useState } from "react";

function Console(props) {
  const [message, setMessage] = useState();
  const [error, setError] = useState();

  let id = props.id;

  const [history, setHistory] = useState();

  async function getCommandData(id_value) {
    var command_data = "No Commands Found";
    try {
      let response = await fetch(
        process.env.REACT_APP_C2URL + "/get_commands",
        {
          method: "POST",
          mode: "cors",
          body: JSON.stringify({
            id: id_value,
          }),
          headers: {
            "Content-Type": "application/json",
          },
        }
      );
      let respJSON = await response.json();
      if (typeof respJSON !== "undefined") {
        command_data = respJSON;
      }
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

  async function organizeHistory() {
    try {
      let command_data = await getCommandData(id);
      setHistory(getCommandData);
    } catch (error) {
      console.log("Error with console!: " + error);
    }
  }

  useEffect(() => {
    organizeHistory();
  }, []);


  return (
    <div>{id}</div>
  );
}

export default Console;
