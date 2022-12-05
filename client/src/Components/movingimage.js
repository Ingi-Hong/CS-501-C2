import "./Stylesheets/movingimage.css";

function MovingImage() {
  let leftValue = Math.floor(Math.random() * 100);
  let animationDur = 3 + Math.floor(Math.random() * 8);
  const styles = ({
    animationName: "mymove",
    animationIterationCount: "infinite",
    animationDirection: "normal",
    animationDuration: animationDur + "s",
    width: "300px",
    height: "auto",
    position: "absolute",
    left: leftValue + "%"
  });
  return (
    <img
      className="moveArrow"
      style={ styles }
      src={require("./Images/macroyal.jpg")}
    ></img>
  );
}

export default MovingImage;
