import { signInWithEmailAndPassword } from "firebase/auth";
import { ReactNode, createContext, useEffect, useState } from "react";
import { auth, db } from "../firebase/firebase-config";
import { onValue, ref } from "firebase/database";

interface AppContextType {
  dht22: any;
  mq135: any;
  pmValue: any;
  setAppData: (value: any) => void;
}

const defaultContext: AppContextType = {
  dht22: null,
  mq135: null,
  pmValue: null,
  setAppData: () => {},
};

export const AppContext = createContext<AppContextType>(defaultContext);

interface Props {
  children: ReactNode;
}

const AppProvider = ({ children }: Props) => {
  const [appData, setAppData] = useState(defaultContext);
  console.log(appData);

  useEffect(() => {
    signInWithEmailAndPassword(auth, "yuzinmontutgo@gmail.com", "yewaitun11621")
      .then((userCredential) => {
        // Signed in
        const user = userCredential.user;

        const candidatesRef = ref(db, `/SensorData/${user.uid}/DHT22`);
        onValue(candidatesRef, (snapshot) => {
          const data = snapshot.val() || {};
          setAppData((prevState) => ({ ...prevState, dht22: data }));
        });

        const votersRef = ref(db, `/SensorData/${user.uid}/mq135`);
        onValue(votersRef, (snapshot) => {
          const data = snapshot.val() || {};
          setAppData((prevState) => ({ ...prevState, mq135: data }));
        });

        const votesRef = ref(db, `/SensorData/${user.uid}/pmValue`);
        onValue(votesRef, (snapshot) => {
          const data = snapshot.val() || {};
          setAppData((prevState) => ({ ...prevState, pmValue: data }));
        });
      })
      .catch((error) => {
        console.error("Error signing in: ", error.message);
      });
  }, []); // Dependencies array is empty to ensure this effect runs only once after the component mounts.

  return (
    <AppContext.Provider value={{ ...appData, setAppData }}>
      {children}
    </AppContext.Provider>
  );
};

export default AppProvider;
