import React from 'react'

const Categories = ({ categories, filteredItems }) => {
	return (
		<div className='btn-container'>
			{categories.map((category, index) => {
				return (
					<button
						key={index}
						className='filter-btn'
						type='button'
						onClick={() => filteredItems(category)}>
						{category}
					</button>
				)
			})}
		</div>
	)
}

export default Categories
