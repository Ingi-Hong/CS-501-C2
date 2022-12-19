import { useEffect, useState } from "react";


function Filedisplay(props){
    id = props.id 
    const [files, setFiles] = useState();
    const [loading, setLoading] = useState();

    async function getData(id){
        setLoading(true);
        try{
            let response = await fetch(process.env.REACT_APP_C2URL + "/get_files", {
                method: "POST",
                mode: "cors",
                body: JSON.stringify({
                  id: id,
                }),
                headers: {
                  "Content-Type": "application/json",
                },
              });
              if (response.statusCode == 200) {
                // Process the response and update the view.
                // Recreate a setTimeout API call which will be fired after 1 second.
                let respJSON = await response.json();
                setFiles(respJSON)
              } else {

              }

        } catch (error) {
            console.log(error)
        } finally{
            setLoading(false);
        }
    }

    useEffect(() => {
        getData(id)
    }, [id])

    if (loading) return <div></div>

    else{
        
    }

}; 

export default Filedisplay;