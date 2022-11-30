import React, { useState } from "react";

function LoginForm() {

  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState("");
  const [message, setMessage] = useState("");



  let handleSubmit = async (e) => {
    e.preventDefault();
    setIsLoading(true);
    try {
      const response = await fetch(
        "https://sea-lion-app-f5nrq.ondigitalocean.app/login",
        {
          method: "POST",
          body: JSON.stringify({
            username: username,
            password: password,
          }),
          headers: {
            "Content-Type": "application/json",
            Accept: "application/json",
          },
        });

        let resJson = await response.json(); 
        if (resJson.status === 200){
            setMessage("Login succesful");
        }
        else{
            setMessage("Error occured when logging in")
        }

    } catch (error) {
      setError(error.message);
    } finally {
      setIsLoading(false);
    }
  };

  if (isLoading) return (<div>Loading...</div>)
  if (error) return (<div>{error}</div>)
  return (
    <div>
        <div>{message}</div>
      <form onSubmit={handleSubmit}>
        <label>
          username: 
          <input
            name="username"
            type="text"
            value={username}
            onChange={(e) => setUsername(e.target.username)}
          />
        </label>
        <br />
        <label>
          password:
          <input
            name="password"
            type="text"
            value={password}
            onChange={(e) => setPassword(e.target.password)}
          />
        </label>
        <button type="submit">Login</button>
      </form>
    </div>
  );
}

export default LoginForm;
