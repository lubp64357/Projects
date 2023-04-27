package de.hska.iwi.ads.solution.sorting.loesung;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

class ReverseArrayTest {

	private ReverseArray<Integer> reverseIntArray;
	private ReverseArray<String> reverseStringArray;


	@BeforeEach
	void init() {
		reverseIntArray = new ReverseArray<Integer>();
		reverseStringArray = new ReverseArray<String>();
	}
	
	@Test
	@DisplayName("Test: the whole array")
	void testReverseFull() {
		Integer[] array = new Integer[] { 0, 1, 2, 3, 4, 5 };
		Integer[] result = new Integer[] { 5, 4, 3, 2, 1, 0 };
		reverseIntArray.reverse(array, 0, 5);
		assertArrayEquals(result, array);
	}
	@DisplayName("Test: whole, not ordered")
	@Test 
	void testReverseRandomFull() {
		Integer[] randomArray = new Integer[] {5, 1, 4, 3, 0, 2};
		reverseIntArray.reverse(randomArray,0,5);
		assertArrayEquals(new Integer[] {2, 0, 3, 4, 1, 5}, randomArray);
	}
	
	@Test 
	@DisplayName("Test: String array")
	void testReverseStringFull() {
		String[] stringArray = new String[] {"Otto", "Anna", "Bob","Hannah"};
		reverseStringArray.reverse(stringArray,0,3);
		assertArrayEquals(new String[] {"Hannah","Bob", "Anna", "Otto"}, stringArray);
	}
	// Testing in range
	@Test 
	@DisplayName("Test: Testing in range at the start")
	void testReverseRangeLeft() {
		Integer[] array = new Integer[] {0, 1, 2, 3, 4, 5};
		reverseIntArray.reverse(array,0,2);
		assertArrayEquals(new Integer[] {2, 1, 0, 3, 4, 5}, array);
	}
	
	@Test 
	@DisplayName("Test: Testing in range at midpoint")
	void testReverseRangeMiddleEven() {
		Integer[] array = new Integer[] {0, 1, 2, 3, 4, 5};
		reverseIntArray.reverse(array,2,3);
		assertArrayEquals(new Integer[] {0, 1, 3, 2, 4, 5}, array);
	}
	
	@Test 
	@DisplayName("Test: Testing in range at the end")
	void testReverseRangeRight() {
		Integer[] array = new Integer[] {0, 1, 2, 3, 4, 5};
		reverseIntArray.reverse(array,3,5);
		assertArrayEquals(new Integer[] {0, 1, 2, 5, 4, 3}, array);
	}
	
	@Test 
	@DisplayName("Test: Illegal arguments")
	void testReverseWrongFromPostitive() {
		Integer[] array = new Integer[] {0,1,2};
		assertThrows(IllegalArgumentException.class, () -> reverseIntArray.reverse(array,42,0));
		assertArrayEquals(new Integer[] {0,1,2}, array);
	}
	@Test 
	@DisplayName("Test: Illegal arguments")
	void testReverseWrongFromNegative() {
		Integer[] array = new Integer[] {0,1,2};
		assertThrows(IllegalArgumentException.class, () -> reverseIntArray.reverse(array,-42,0));
		assertArrayEquals(new Integer[] {0,1,2}, array);
	}
	
}
