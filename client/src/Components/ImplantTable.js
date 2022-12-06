import "spectre.css";

function ImplantTable() {
  return (
    <table className="table table-hover table-striped table-scroll ">
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
    </table>
  );
}

export default ImplantTable;
