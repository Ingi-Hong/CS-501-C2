import { useEffect, useState } from "react";
import "spectre.css";
import "./Stylesheets/implanttable.css";

// Used https://www.pluralsight.com/guides/fetch-data-from-a-json-file-in-a-react-app
function ImplantTable(props) {
  const [isLoading, setIsLoading] = useState();
  const [error, setError] = useState();
  const [message, setMessage] = useState();
  const [data, setData] = useState();
  
  let setID = props.setID;

  const getData = async () => {
    try {
      setIsLoading(true);
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

  function handleTableClick(e){
    setID(e.currentTarget.getAttribute("value")[0]);
  };

  useEffect(() => {
    getData();
  }, []);

  if (isLoading) return <div className="loading" />
  return (
    <div className="wrapper">
      <h1>Implant Table</h1>
      <h2>{error} {message}</h2>
      <table className="table table-hover table-striped">
        <thead>
          <tr>
            <th>ID</th>
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
              <tr value={item} onClick={(e) => handleTableClick(e)} key={iterator} className="active">
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
