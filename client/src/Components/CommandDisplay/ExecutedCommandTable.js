function ExecutedCommandTable(props) {
  console.log(props.commandList);
  var commandList = props.commandList;
  return (
    <div
      style={{
        maxHeight: "500px",
        overflowWrap: "normal",
        overflowX: "scroll",
      }}
    >
      <table
        className="table"
        style={{ tableLayout: "fixed", width: "100%", overflowWrap: "normal" }}
      >
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
                <td style={{ width: "1px", whiteSpace: "nowrap" }}>
                  {commands[0]}
                </td>
                <td style={{ wordWrap: "break-word", width: "100%" }}>
                  {commands[2]}
                </td>
                <td>{commands[3]}</td>
                <td style={{ wordWrap: "break-word", width: "100%" }}>
                  {commands[5]}
                </td>
                <td>{commands[7]}</td>
                <td>{commands[6].toString()}</td>
              </tr>
            ))}
        </tbody>
      </table>
    </div>
  );
}

export default ExecutedCommandTable;
