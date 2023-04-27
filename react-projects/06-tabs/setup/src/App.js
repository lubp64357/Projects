import React, { useState, useEffect } from 'react'
import { FaAngleDoubleRight } from 'react-icons/fa'
// ATTENTION!!!!!!!!!!
// I SWITCHED TO PERMANENT DOMAIN
const url = 'https://course-api.com/react-tabs-project'
function App() {
	const [loading, setLoading] = useState(true)
	const [jobs, setJobs] = useState([])
	const [value, setValue] = useState(0)

	const fetchJobs = async () => {
		const response = await fetch(url)
		const data = await response.json()
		setJobs(data)
		setLoading(false)
	}
	useEffect(() => {
		fetchJobs()
	}, [])

	if (loading) {
		return <h2>loading...</h2>
	}
	const { company, dates, duties, title } = jobs[value]
	return (
		<section>
			<div className='title'>
				<h2>experience</h2>
				<div className='underline'></div>
			</div>
			<div className='jobs-center'>
				<div className='btn-container'>
					{jobs.map((job, index) => {
						return (
							<button
								key={index}
								onClick={() => setValue(index)}
								className={`job-btn ${index === value && 'active-btn'} `}>
								{job.company}
							</button>
						)
					})}
				</div>

				<article className='job-info'>
					<h3>{title}</h3>
					<h4>{company}</h4>
					<p className='job-dates'>{dates}</p>
					{duties.map((duty, index) => {
						return (
							<div key={index} className='job-description'>
								<FaAngleDoubleRight className='jon-icon'></FaAngleDoubleRight>
								<p>{duty}</p>
							</div>
						)
					})}
				</article>
			</div>
		</section>
	)
	return <h2>tabs project setup</h2>
}

export default App
