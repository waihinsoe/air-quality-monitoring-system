import { useContext } from "react";
import "./App.css";
import { AppContext } from "./context/AppContext";
import Progress, { ProgressProps } from "antd/es/progress";
import Title from "antd/es/typography/Title";
import { Tag, Typography } from "antd";
import LogoLeft from "./assets/file (9).png";
import LogoRight from "./assets/file (10).png";

const conicColors: ProgressProps["strokeColor"] = {
  "0%": "#87d068",
  "50%": "#ffe58f",
  "100%": "#ffccc7",
};

const assessQualityColor = (quality: string) => {
  if (quality === "Good") {
    return "green";
  } else if (quality === "Moderate") {
    return "yellow";
  } else if (
    quality === "Poor" ||
    quality === "Unhealthy for Sensitive Groups"
  ) {
    return "orange";
  } else if (quality === "Unhealthy") {
    return "red";
  } else if (quality === "Very Unhealthy") {
    return "purple";
  } else {
    return "maroon";
  }
};

function App() {
  const { dht22, mq135, pmValue } = useContext(AppContext);
  return (
    <div
      style={{
        width: "100%",
        height: "100vh",
        maxWidth: 1000,
        margin: "0 auto",
        display: "flex",
        flexDirection: "column",
        gap: 40,
      }}
    >
      <div
        style={{
          display: "flex",
          justifyContent: "space-evenly",
          alignItems: "center",
          marginTop: 20,
        }}
      >
        <img src={LogoLeft} alt="logoLeft" width={70} height={70} />
        <Title
          className="title"
          style={{
            textAlign: "center",
            marginTop: 10,
          }}
        >
          Air Quality Monitoring System
        </Title>
        <img src={LogoRight} alt="logoRight" width={70} height={70} />
      </div>

      <div
        style={{
          display: "flex",
          justifyContent: "space-evenly",
          alignItems: "center",
        }}
      >
        <div
          style={{
            width: 200,
            height: 200,
            display: "flex",
            flexDirection: "column",
            alignItems: "center",
            justifyContent: "center",
            borderRadius: 10,
            boxShadow: "rgba(100, 100, 111, 0.2) 0px 7px 29px 0px",
          }}
        >
          <Title level={4}>Temperature</Title>
          <Progress
            type="dashboard"
            percent={dht22 ? dht22.temC : 0}
            strokeColor={conicColors}
            status="active"
            format={(percent) => `${percent}°C`}
          />
        </div>

        <div
          style={{
            width: 200,
            height: 200,
            display: "flex",
            flexDirection: "column",
            alignItems: "center",
            justifyContent: "center",
            borderRadius: 10,
            boxShadow: "rgba(100, 100, 111, 0.2) 0px 7px 29px 0px",
          }}
        >
          <Title level={4}>Humidity</Title>
          <Progress
            type="dashboard"
            percent={dht22 ? dht22.humi : 0}
            strokeColor={conicColors}
            status="active"
          />
        </div>
      </div>
      <div style={{ display: "flex", justifyContent: "space-evenly" }}>
        <div
          style={{
            width: 200,
            height: 200,
            display: "flex",
            flexDirection: "column",
            alignItems: "center",
            justifyContent: "center",
            borderRadius: 10,
            boxShadow: "rgba(100, 100, 111, 0.2) 0px 7px 29px 0px",
          }}
        >
          <Tag color={mq135 ? assessQualityColor(mq135.quality) : "default"}>
            {mq135 ? mq135.quality : "---"}
          </Tag>
          <Typography>
            <span style={{ fontWeight: "bold", fontSize: 24 }}>
              {mq135 ? mq135.value.toFixed(2) : 0}
            </span>
            ppm
          </Typography>
          <Typography>CO2</Typography>
        </div>
        <div
          style={{
            width: 200,
            height: 200,
            display: "flex",
            flexDirection: "column",
            alignItems: "center",
            justifyContent: "center",
            borderRadius: 10,
            boxShadow: "rgba(100, 100, 111, 0.2) 0px 7px 29px 0px",
          }}
        >
          <Tag
            color={pmValue ? assessQualityColor(pmValue.quality) : "default"}
          >
            {pmValue ? pmValue.quality : "---"}
          </Tag>
          <Typography>
            <span style={{ fontWeight: "bold", fontSize: 24 }}>
              {pmValue ? pmValue.PM_2p5.toFixed(2) : 0}
            </span>
            µg/m³
          </Typography>
          <Typography>PM 2.5</Typography>
        </div>
      </div>
    </div>
  );
}

export default App;
