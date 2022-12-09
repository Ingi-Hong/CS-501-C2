import { useState } from 'react';
import { read_cookie } from 'sfcookies';

function QueueCommandForm(props) {
    var setRefresh = props.setRefresh;
    const [command, setCommand] = useState("");
    const [isLoading, setIsLoading] = useState(false);
    const [error, setError] = useState("");
    const [message, setMessage] = useState("");

    let target_implant_id = props.target_implant_id;
    let setWasClick = props.setWasClick;
    let handleSubmit = async (e) => {
      e.preventDefault();
      setIsLoading(true);
      try {
        let response = await fetch(process.env.REACT_APP_C2URL + "/queueCommand", {
          method: "POST",
          mode: "cors",
          body: new URLSearchParams({
            target_implant_id: target_implant_id,
            command: command,
          }),
          headers: {
            "Content-Type": "application/x-www-form-urlencoded",
            'Access-Control-Allow-Origin': '*',
            'Accept': 'application/json',
            'Authorization': 'Bearer ' + read_cookie('access_token') 
          }
        });
        if (response.status === 200) {
          setMessage("Command succesfully queued");
        }  else {
          setMessage("Error when submitting form");
        }
        setError("");
        setWasClick(command + target_implant_id + Math.random());
      } catch (error) {
        setError(error.message);
      } finally {
        setIsLoading(false);
        setRefresh("refreshed");
      }
    };
  
    if (isLoading) return <div className='loading'>Loading...</div>;

    return (
      <div className='command-wrapper'>
        {error && <div>error: {error}</div>}
        <div>{message}</div>
        <form onSubmit={handleSubmit} className="form-group">
          <label className='form-label' htmlFor='command'>
            command:
            <input
            id='command'
            className='form-input'
              name="command"
              type="text"
              value={command}
              onChange={(e) => setCommand(e.target.value)}
            />
          </label>
          <button className='btn' type="submit">Submit command</button>
        </form>
      </div>
    );
  };

  export default QueueCommandForm;