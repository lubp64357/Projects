
import './App.css'
import 'bootstrap/dist/css/bootstrap.min.css'
import Navbar from './components/Navbar.js'
import Default from './components/Default.js'
import Cart from './components/Cart'
import Details from './components/Details.js'
import ProductList from './components/ProductList.js'
import { Routes, Route } from 'react-router-dom'
import Modal from './components/Modal'

function App() {
	return (
		<>
			<Navbar />
			<Routes>
				<Route exact path='/' element={<ProductList />} />

				<Route exact path='/details' element={<Details />} />
				<Route exact path='/cart' element={<Cart />} />
				<Route path='*' element={<Default />} />
			</Routes>
			<Modal />
		</>
	)
}

export default App
