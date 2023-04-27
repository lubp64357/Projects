package de.hska.iwi.ads.solution.sorting.loesung;

import de.hska.iwi.ads.sorting.Reverse;

public class ReverseArray<E extends Comparable<E>> implements Reverse<E> {

	@Override
	public void reverse(E[] a, int from, int to) {
		// TODO Auto-generated method stub
		
		if(from >= a.length 
				|| to >= a.length
				|| from < 0
				|| to < 0) {
			throw new IllegalArgumentException();	
		}
		for(int i = 0; i < (to - from + 1) / 2; i++) {
			// first element(from+i)  with last element (to - i)
			E tmp = a [i + from];
			a[i + from]= a[to -i];
			a[to -i] = tmp;
		}
	}
	
	public static void main(String[] args) {
		ReverseArray<Integer> reverseIntArray = new ReverseArray();
		Integer[] ascendingArray = new Integer[] { 0, 1, 2, 3, 4, 5 };
		reverseIntArray.reverse(ascendingArray, 0, 5);
	}

}
