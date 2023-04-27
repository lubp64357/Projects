import React, { useState } from 'react'
import people from './data'
import { FaChevronLeft, FaChevronRight, FaQuoteRight } from 'react-icons/fa'

const Review = () => {
	const [index, setIndex] = useState(0)
	const { name, job, image, text } = people[index]
	const prevPerson = () => {
		setIndex(() => {
			let newIndex = index - 1
			return newIndex < 0 ? people.length - 1 : newIndex
		})
	}
	const nextPerson = () => {
		setIndex(() => {
			let newIndex = index + 1
			return newIndex >= people.length ? 0 : newIndex
		})
	}
	const rndPerson = () => {
		setIndex(() => {
			let newIndex = Math.floor(Math.random() * people.length)
			if (newIndex === index) {
				newIndex += 1
			} // can happen an overflow for the array solved with another function checkNumbers
			return newIndex
		})
	}
	return (
		<article className='review'>
			<div className='img-container'>
				<img src={image} alt={name} className='person-img' />
				<span className='quote-icon'>
					<FaQuoteRight />
				</span>
			</div>
			<h4 className='author'>{name}</h4>
			<p className='job'>{job}</p>
			<p className='info'>{text}</p>
			<div className='button-container'>
				<button className='prev-btn' onClick={prevPerson}>
					<FaChevronLeft></FaChevronLeft>
				</button>
				<button className='next-btn' onClick={nextPerson}>
					<FaChevronRight></FaChevronRight>
				</button>
			</div>
			<button className='random-btn' onClick={rndPerson}>
				Surprise me
			</button>
		</article>
	)
}

export default Review
