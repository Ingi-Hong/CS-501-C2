import { useState, useEffect, useCallback } from "react";
import "spectre.css";

function CommandDisplay() {
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
      if (typeof respJSON !== 'undefined'){
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
      console.log(implantStuff)
      const array = await Promise.all(
        implantStuff.map((element) =>
          (getCommandData(element[0]))
        )
      );
      
      setDisplayItems(array);
    } catch (error) {
      console.log("ERROR FETCHING DATA: " + error);
      setError(error.message);
    } finally {
      setIsLoading(false);
    }
  });

  useEffect(() => {
    getDisplayItems();
  }, []);

  if (isLoading) return <div class="loading loading-lg"></div>;
  if (error)
    return (
      <div>
        error: {error} {console.log(error)}{" "}
      </div>
    );
  return (
    <div>

      {
        
        displayItems && displayItems.map( (commandList, id) => (
          <div>
          <div class="divider"></div>
          <div className="accordion">   
            <input type="checkbox" id={id+1} name="accordion-checkbox" hidden />
            <label className="accordion-header" for={id+1}>
            <i class="icon icon-arrow-right mr-1"></i>
            Implant: {id + 1}
            </label>
            <div className="accordion-body">

                {commandList}

              </div> 
          </div>
          </div>
        ))
      }
      </div>
      
  );
}

export default CommandDisplay;
