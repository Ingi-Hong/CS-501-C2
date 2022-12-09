import "spectre.css"
import { useState } from 'react';

function TestImplantFun() {
    
    const [sleep, setImplantID] = useState("");
    const [isLoading, setIsLoading] = useState(false);
    const [error, setError] = useState("");
    const [message, setMessage] = useState("");
  
    let handleSubmit = async (e) => {
      e.preventDefault();
      setIsLoading(true);
      try {
        let response = await fetch(process.env.REACT_APP_C2URL + "/register_implant", {
          method: "POST",
          mode: "cors",
          body: JSON.stringify({
            sleep: sleep,
            jitter: 5
          }),
          headers: {
            "Content-Type": "application/json",
            'Access-Control-Allow-Origin': '*',
            'Accept': 'application/json'
          }
        });
        if (response.status === 200) {
          setMessage("Succesfully Registered");
        }  else {
          setMessage("Error when submitting form");
        }
      } catch (error) {
        setError(error.message);
      } finally {
        setIsLoading(false);
      }
    };
  
    if (isLoading) return <div>Loading...</div>;

    return (
      <div>
        {error && <div>error: {error}</div>}
        <div>{message}</div>
        <form onSubmit={handleSubmit} className="form-group">
          <label>
            Sleep value:
            <input
              name="sleep"
              type="text"
              value={sleep}
              onChange={(e) => setImplantID(e.target.value)}
            />
          </label>
          <br />
        </form>
      </div>
    );
  };

  export default TestImplantFun;