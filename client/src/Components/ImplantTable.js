import { useEffect, useState } from "react";
import "spectre.css";
import "./Stylesheets/implanttable.css";

// Used https://www.pluralsight.com/guides/fetch-data-from-a-json-file-in-a-react-app
function ImplantTable() {
  const [isLoading, setIsLoading] = useState();
  const [error, setError] = useState();
  const [message, setMessage] = useState();
  const [data, setData] = useState();
  const getData = async () => {
    setIsLoading(true);
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
      setData(respJSON);
      console.log(respJSON);
      if (response.status === 200) {
      } else {
        setMessage("Error getting implant data");
      }
    } catch (error) {
      setError(error.message);
    } finally {
      setIsLoading(false);
    }
  };

  useEffect(() => {
    getData();
  }, []);

  return (
    <div className="wrapper">
      <h1>Implant Table</h1>
      <h2>{error}</h2>
      <table className="table table-hover table-striped">
        <thead>,
          <tr>
            <th>First Connect</th>
            <th>Active?</th>
            <th>Location</th>
            <th>Computer Name</th>
            <th>Computer GUID</th>
            <th>IP Address</th>
            <th>Last Seen</th>
            <th>Jitter</th>
            <th>Next Check In</th>
            <th>Sleep</th>
            <th>Session Key</th>
            <th>ID</th>
          </tr>
        </thead>
        {/*
          FIrst_connection
          active
          location
          computer name
          guid
          ip address
          last seen 
          jitter
          next check in
          sleep
          sesh key 
           ImplantID*/}
        <tbody>
          {data &&
            data.map((item, iterator) => (
              <tr key={iterator} className="active">
                {item.map((currentData, iterator) => (
                    <td key={iterator}>{currentData}</td>
                ))}
              </tr>
            ))}
        </tbody>
      </table>
    </div>
  );
}

export default ImplantTable;
