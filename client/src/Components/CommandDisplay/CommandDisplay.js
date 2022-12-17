import { useState, useEffect, useCallback } from "react";
import "spectre.css";
import "../Stylesheets/spectre-icons.min.css";
import ExecutedCommandTable from "./ExecutedCommandTable";
import ExecutingCommandTable from "./ExecutingCommandTable";
import UntouchedCommandTable from "./UntouchedCommandTable";

function CommandDisplay(props) {
  var refresh = props.refresh;
  var setID = props.setID;

  const [isLoading, setIsLoading] = useState();
  const [error, setError] = useState();
  const [message, setMessage] = useState();
  const [displayItems, setDisplayItems] = useState();

  const getImplants = useCallback(async () => {
    var implantStuff;
    try {
      let response = await fetch(
        process.env.REACT_APP_C2URL + "/display_implants",
        {
          method: "GET",
          mode: "cors",
          headers: {
            "Content-Type": "application/json",
          },
        }
      );
      let respJSON = await response.json();
      implantStuff = respJSON;
      if (response.status === 200) {
      } else {
        setMessage("Error getting implant data");
      }
    } catch (error) {
      setError(error.message);
    } finally {
      return implantStuff;
    }
  });

  async function getCommandData(id_value) {
    var command_data = "No Commands ";
    try {
      let response = await fetch(
        process.env.REACT_APP_C2URL + "/client_get_commands",
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

  const getDisplayItems = useCallback(async () => {
    try {
      setIsLoading(true);

      var implantStuff = await getImplants();
      console.log(implantStuff);
      const array = await Promise.all(
        implantStuff.map((element) => getCommandData(element[0]))
      );
      var filtered_commands = array.map((commandList) => {
        return filterCommandList(commandList);
      });
      setDisplayItems(filtered_commands);
    } catch (error) {
      console.log("ERROR FETCHING DATA: " + error);
      setError(error.message);
    } finally {
      setIsLoading(false);
    }
  });

  function filterCommandList(commandList) {
    var untouched = commandList.filter((command, commandIndex) => {
      if (command[4] === "untouched") {
        return true;
      }

      return false;
    });

    var executing = commandList.filter((command, commandIndex) => {
      if (command[4] === "executing") {
        return true;
      }

      return false;
    });

    var executed = commandList.filter((command, commandIndex) => {
      if (command[4] === "executed") {
        return true;
      }

      return false;
    });

    return { untouched: untouched, executing: executing, executed: executed };
  }

  useEffect(() => {
    getDisplayItems();
  }, []);

  if (isLoading) return <div className="loading loading-lg"></div>;
  if (error) return <div>error: {error}</div>;
  return (
    <div className="p-centered">
      {displayItems &&
        displayItems.map((implant, id) => (
          <div key={id + "wrapper"}>
            <div className="divider" key={id + "divider"}></div>
            <div className="accordion" key={id + "accordion"}>
              <input
                key={id + 1}
                type="checkbox"
                id={id + 1}
                name="accordion-checkbox"
                hidden
              />
              <label
                className="accordion-header"
                htmlFor={id + 1}
                key={id + "header"}
              >
                <i className="icon icon-arrow-right mr-1"></i> Implant: {id + 1}
              </label>
              <div className="accordion-body mx-1" key={id + "body"}>
                <div className="container">
                  <div className="columns m-2 p-2">
                    <div className="column col-12">
                      <h4>Untouched</h4>
                      {implant.untouched && (
                        <UntouchedCommandTable
                          commandList={implant.untouched}
                        />
                      )}
                    </div>

                    <div className="column col-12">
                      <h4>Executing</h4>
                      {implant.executing && (
                        <ExecutingCommandTable
                          commandList={implant.executing}
                        />
                      )}
                    </div>

                    <div className="column col-12">
                      <h4>Executed</h4>
                      {implant.executed && (
                        <ExecutedCommandTable commandList={implant.executed} />
                      )}
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        ))}
    </div>
  );
}

export default CommandDisplay;
