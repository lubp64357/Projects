import React, { useState, useEffect } from 'react'
import Loading from './Loading'
import Tours from './Tours'
// ATTENTION!!!!!!!!!!
// I SWITCHED TO PERMANENT DOMAIN
const url = 'https://course-api.com/react-tours-project'
function App() {
	const [tours, setTours] = useState([])
	const [loading, setLoading] = useState(true)
	const [error, setError] = useState(null)
	const removeTour = (id) => {
		const newTours = tours.filter((tour) => tour.id !== id)
		setTours(newTours)
	}
	const fetchData = async () => {
		try {
			const response = await fetch(url)
			const data = await response.json()
			setTours(data)
			setLoading(false)
		} catch (error) {
			setError(error)
			setLoading(false)
		}
	}
	useEffect(() => {
		fetchData()
	}, [])

	if (loading) {
		return (
			<main>
				<Loading />
			</main>
		)
	}
	if (tours.length === 0) {
		return (
			<main>
				<div className='title'>
					<h2>No tours left</h2>
					<button className='btn ' onClick={() => fetchData()}>
						refresh
					</button>
				</div>
			</main>
		)
	}
	if (tours) {
		return (
			<main>
				<Tours tours={tours} removeTour={removeTour} />
			</main>
		)
	}

	if (error) {
		return <p>Error: {error.message}</p>
	}
}

export default App
