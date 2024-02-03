import Header from './Header';
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import Signin from './pages/Signin';
import Post from './pages/Post';
import Control from './pages/control';


function App(){
    return(
        <BrowserRouter>
            <Header />
                <Routes>
                    <Route path="/" element={<Post />}></Route>
                    <Route path='/signin' element={<Signin />} ></Route>
                    <Route path="/control" element={<Control />} ></Route>
                </Routes>
        </BrowserRouter>
    );
}


export default App;