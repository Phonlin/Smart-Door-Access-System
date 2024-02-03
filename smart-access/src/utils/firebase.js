import firebase from 'firebase/compat/app';
import 'firebase/compat/auth';



const firebaseConfig = {
    apiKey: "AIzaSyD-b2ypEVgtn_kFwldA5lXokhG5zbzSWTs",
    authDomain: "smart-access-2316b.firebaseapp.com",
    projectId: "smart-access-2316b",
    storageBucket: "smart-access-2316b.appspot.com",
    messagingSenderId: "1038168839799",
    appId: "1:1038168839799:web:3d8a0ecb789448fccd1872"
  };


firebase.initializeApp(firebaseConfig);

export default firebase;