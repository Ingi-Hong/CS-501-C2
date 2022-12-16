function ExecutedCommandTable(props) {
    var commandList = props.commandList;
    return (
      <table className="table">
        <thead>
          <tr>
            <th>Task ID</th>
            <th>Command</th>
            <th>Created On</th>
            <th>Response Data</th>
            <th>Recieved</th>
            <th>Success</th>
          </tr>
        </thead>
        <tbody>
          {commandList &&
            commandList.map((commands) => (
              <tr className="active" key={commands}>
                <td>{commands[0]}</td>
                <td>{commands[2]}</td>
                <td>{commands[3]}</td>
                <td>{commands[5]}</td>
                <td>{commands[-2]}</td>
                <td>{commands[-3]}</td>
              </tr>
            ))}
        </tbody>
      </table>
    );
  }
  
  export default ExecutedCommandTable;
  