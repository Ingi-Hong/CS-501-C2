import { useEffect, useRef, useState } from "react";
import "../fonts/JetBrainsMono-Light.ttf";
import "./Stylesheets/console.css";
import QueueCommandForm from "./QueueCommandForm";

// https://bobbyhadz.com/blog/javascript-date-remove-local-timezone
function padTo2Digits(num) {
  return num.toString().padStart(2, "0");
}

function formatDate(date) {
  date = new Date(date);

  return (
    [
      date.getFullYear(),
      padTo2Digits(date.getMonth() + 1),
      padTo2Digits(date.getDate()),
    ].join("-") +
    " " +
    [
      padTo2Digits(date.getHours()),
      padTo2Digits(date.getMinutes()),
      padTo2Digits(date.getSeconds()),
    ].join(":")
  );
}



function Console(props) {
  const [message, setMessage] = useState();
  const [error, setError] = useState();
  const [loading, setLoading] = useState(true);
  const [wasClick, setWasClick] = useState();
  const endWindow = useRef(null);
  const focusRef = useRef(null);

  //https://stackoverflow.com/questions/37620694/how-to-scroll-to-bottom-in-react
  const scrollToBottom = () => {
    if (endWindow.current) {
      endWindow.current.scrollIntoView({
        behavior: "smooth",
        block: "nearest",
      });
    }
  };

  let id = props.id;

  const [history, setHistory] = useState({
    sorted: { creator: "", time: "", command: "", response: "" },
  });

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
    } finally {
      return command_data;
    }
  }

  async function organizeHistory(id_value) {
    setLoading(true);
    try {
      let command_data = await getCommandData(id_value);
      console.log("This is command data: " + command_data);
      setHistory(command_data);
    } catch (error) {
      console.log("Error with console!: " + error);
    } finally {
      setLoading(false);
    }
  }



  // Code for polling
// https://stackoverflow.com/a/63134447
let apiTimeout = setTimeout(fetchAPIData, 1000);

async function fetchAPIData() {
  if (!id) {
    setTimeout(fetchAPIData, 10000);
  } else {
    let response = await fetch(process.env.REACT_APP_C2URL + "/get_history", {
      method: "POST",
      mode: "cors",
      body: JSON.stringify({
        id: id,
      }),
      headers: {
        "Content-Type": "application/json",
      },
    });
    if (response.statusCode == 200) {
      // Process the response and update the view.
      // Recreate a setTimeout API call which will be fired after 1 second.
      let respJSON = await response.json();
      setHistory(respJSON)
      apiTimeout = setTimeout(fetchAPIData, 10000);
    } else {
      clearTimeout(apiTimeout);
      // Failure case. If required, alert the user.
    }
  }
}



  const focusOnRef = () => {
    focusRef.current.focus();
  };

  useEffect(() => {
    organizeHistory(id);
  }, [id, wasClick]);

  useEffect(() => {
    scrollToBottom();
  }, [wasClick]);

  // combined = [{"sender": "user", "creator":x[-1], "time":x[3], "command":x[2] } for x in executed]
  // combined += [{"sender": "implant", "time":x[-2], "command":x[2], "response":x[-4]} for x in executed]
  if (loading)
    return (
      <div id="console-wrapper">
        {error} {message}
        <h1 className="p-centered">Implant {id}</h1>
        <div className="p-centered" id="console-window"></div>
      </div>
    );
  return (
    <div id="console-wrapper">
      {error} {message}
      <h1 className="p-centered">Implant {id}</h1>
      <div className="p-centered" id="console-window" onClick={focusOnRef}>
        {/* <Content loading={loading} history={history} id={id}/> */}
        <div>
          {history.sorted &&
            history.sorted.map((dialogue, key) => {
              if (dialogue.sender === "user") {
                return (
                  <div key={key}>
                    [{formatDate(dialogue.time)}] [{dialogue.creator}]{" "}
                    {dialogue.command}
                  </div>
                );
              } else {
                return (
                  <div key={key}>
                    [{formatDate(dialogue.time)}] [implant {id}]{" "}
                    {dialogue.response}
                  </div>
                );
              }
            })}
          <div ref={endWindow}></div>
          <QueueCommandForm
            ref_val={focusRef}
            target_implant_id={id}
            setWasClick={setWasClick}
          />
        </div>
      </div>
    </div>
  );
}

export default Console;
