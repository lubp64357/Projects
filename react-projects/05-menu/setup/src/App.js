import React, { useState } from 'react'
import Menu from './Menu'
import Categories from './Categories'
import items from './data'

const allCategories = ['all', ...new Set(items.map((item) => item.category))]
console.log(allCategories)

function App() {
	const [menuItems, setMenuItems] = useState(items)
	const [categories, setCategories] = useState(allCategories)

	const filteredItems = (category) => {
		if (category === 'all') {
			setMenuItems(items)
			return
		}
		const newItems = items.filter((item) => item.category === category)
		setMenuItems(newItems)
	}
	return (
		<main>
			<section className='menu'>
				<div className='title'>
					<h2>Our menu</h2>
					<div className='underline'></div>
				</div>
				<Categories categories={categories} filteredItems={filteredItems} />
				<Menu items={menuItems} />
			</section>
		</main>
	)
}

export default App
