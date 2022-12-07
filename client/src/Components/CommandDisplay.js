import { useState, useEffect } from 'react';

function CommandDisplay(){
    const [isLoading, setIsLoading] = useState();
    const [error, setError] = useState();
    const [message, setMessage] = useState();
    const [data, setData] = useState();

    const getData = async () => {
      setIsLoading(true);
      try {
        let response = await fetch(
          process.env.REACT_APP_C2URL + "/get_commands",
          {
            method: "POST",
            mode: "cors",
            body: JSON.stringify({
              id: 1
            }),
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
        console.log(data);
      }
    };
  
    useEffect(() => {
      getData();
    }, []);

    return (
        <div>This is data: {data}</div>
    );
};


export default CommandDisplay;