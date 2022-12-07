import { useState } from 'react';

function QueueCommandForm(props) {
     {/* target_implant_id = request.form.get('implantID')
        command = request.form.get('command')
        created_on = datetime.now()
        status = "untouched" */}
    var setRefresh = props.setRefresh;
    const [target_implant_id, setImplantID] = useState("");
    const [command, setCommand] = useState("");
    const [isLoading, setIsLoading] = useState(false);
    const [error, setError] = useState("");
    const [message, setMessage] = useState("");
  
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
            'Accept': 'application/json'
          }
        });
        if (response.status === 200) {
          setMessage("Command succesfully queued");
        }  else {
          setMessage("Error when submitting form");
        }
        setError("");
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
          <label className='form-label' htmlFor='implant-ID'>
            Implant ID:
            <input
            className='form-input'
            id='implant-ID'
              name="implant-ID"
              type="text"
              value={target_implant_id}
              onChange={(e) => setImplantID(e.target.value)}
            />
          </label>
          <br />
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