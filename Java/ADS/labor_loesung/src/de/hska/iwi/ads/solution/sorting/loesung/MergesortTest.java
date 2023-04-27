package de.hska.iwi.ads.solution.sorting.loesung;

import static org.junit.Assert.assertArrayEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

class MergesortTest {
	private Mergesort<Integer> mergesortInt;
	

	@BeforeEach
	void init() {
		mergesortInt = new Mergesort<Integer>();
	}
	// general test
	@Test
	@DisplayName("Test: Normal random array")
	void testMergesortIntArray1l() {
		Integer[] a = { 2, 1, 7, 12, 5, 9, 8, 10 };
		Integer[] result = { 1, 2, 5, 7, 8, 9, 10, 12 };
		mergesortInt.sort(a);
		assertArrayEquals(result, a);
	}
	
	@Test
	@DisplayName("Test: Only 1 element")
	void testMergesortOneElement() {
		Integer[] a = { 5 };
		Integer[] result = { 5 };
		mergesortInt.sort(a);
		assertArrayEquals(result, a);
	}	
	
	@Test
	@DisplayName("Test: Already in order")
	void testMergesortAlreadySortedList() {
		Integer[] a = { 1, 2, 3, 4, 5, 6};
		Integer[] result = { 1, 2, 3, 4, 5, 6};
		mergesortInt.sort(a);
		assertArrayEquals(result, a);
	}
	
	@Test
	@DisplayName("Test: Reversed")
	void testMergesortReverseSortedList() {
		Integer[] a = {6, 5, 4, 3, 2, 1,};
		Integer[] result = {1, 2, 3, 4, 5, 6,};
		mergesortInt.sort(a);
		assertArrayEquals(result, a);
	}
	
	@Test
	@DisplayName("Test: With one element repeatable")
	void testMergesortAllSameElement() {
		Integer[] a = { 7, 7, 7, 7, 7};
		Integer[] result = { 7, 7, 7, 7, 7};
		mergesortInt.sort(a);
		assertArrayEquals(result, a);
	}
	@Test
	@DisplayName("Test: Array with even numer .length()")
	void testMergesortArrayEvenNumber() {
		Integer[] a = { 5, 7, 6, 4, 8, 3, 1};
		Integer[] result = { 1, 3, 4, 5, 6 ,7, 8};
		mergesortInt.sort(a);
		assertArrayEquals(result, a);
	}
	@Test
	@DisplayName("Test: Array with odd number .length")
	void testMergesortArrayOddNumber() {
		Integer[] a = {5, 7, 6, 4, 8, 3, 1, 10};
		Integer[] result = { 1, 3, 4, 5, 6 ,7, 8, 10};
		mergesortInt.sort(a);
		assertArrayEquals(result, a);
	}
	
	
	
	}