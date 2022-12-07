import "../Stylesheets/movingimage.css";

function MovingImage() {
  const images = [require("../Images/macroyal.jpg"), require("../Images/macrun.jpeg"), require("../Images/macyawn.jpg")];
  let pick = Math.floor(Math.random()*3);
  let leftValue = (Math.random() * 90) - 4;
  let animationDur = 3 + (Math.random() * 10);
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
      src={images[pick]}
    ></img>
  );
}

export default MovingImage;
