function ExecutingCommandTable(props) {
  var commandList = props.commandList;
  return (
    <div
      style={{
        maxHeight: "1000px",
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
          </tr>
        </thead>
        <tbody>
          {commandList &&
            commandList.map((commands) => (
              <tr className="active" key={commands}>
                <td>{commands[0]}</td>
                <td style={{ wordWrap: "break-word" }}>{commands[2]}</td>
                <td>{commands[3]}</td>
              </tr>
            ))}
        </tbody>
      </table>
    </div>
  );
}

export default ExecutingCommandTable;
