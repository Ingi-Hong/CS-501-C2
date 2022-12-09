import { useEffect, useState, useRef } from "react";
import { read_cookie } from "sfcookies";
import "./Stylesheets/form.css";

function QueueCommandForm(props) {
  var setRefresh = props.setRefresh;
  const [command, setCommand] = useState("");
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState("");
  const [message, setMessage] = useState("");

  const ref = props.ref_val;

  let target_implant_id = props.target_implant_id;
  let setWasClick = props.setWasClick;
  let handleSubmit = async (e) => {
    e.preventDefault();
    setIsLoading(true);
    try {
      let response = await fetch(
        process.env.REACT_APP_C2URL + "/queueCommand",
        {
          method: "POST",
          mode: "cors",
          body: new URLSearchParams({
            target_implant_id: target_implant_id,
            command: command,
          }),
          headers: {
            "Content-Type": "application/x-www-form-urlencoded",
            "Access-Control-Allow-Origin": "*",
            Accept: "application/json",
            Authorization: "Bearer " + read_cookie("access_token"),
          },
        }
      );
      if (response.status === 200) {
        setMessage("Command succesfully queued");
      } else {
        setMessage("Error when submitting form");
      }
      setError("");
      setWasClick(command + target_implant_id + Math.random());
    } catch (error) {
      setError(error.message);
      e.focus();
    } finally {
      setIsLoading(false);
      setRefresh("refreshed");
      e.focus();
    }
  };

  if (isLoading) return <div className="loading">Loading...</div>;

  return (
    <div className="d-flex">

    <div>
      
      <form className="form" onSubmit={handleSubmit}>
        $
        <input
          ref={ref}
          id="command"
          className="regular-joe"
          name="command"
          type="text"
          value={command}
          onChange={(e) => setCommand(e.target.value)}
          />
      </form>
          </div>
      {error && <div>error: {error}</div>}
      <div>{message}</div>
    </div>
  );
}

export default QueueCommandForm;
