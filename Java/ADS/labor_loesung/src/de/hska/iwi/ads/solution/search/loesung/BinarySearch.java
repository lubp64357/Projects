package de.hska.iwi.ads.solution.search.loesung;

import de.hska.iwi.ads.search.Search;

public class BinarySearch<E extends Comparable<E>> implements Search<E> {

	@Override
	public int search(E[] a, E key, int left, int right) {
		// TODO Auto-generated method stub
		int mid = (left + right) / 2;
		
		if (left > right) { // key nicht in a enthalten ist
			if (a.length == left) { // key > a[a.length -1]
				return left;
			} else if (a[left].compareTo(key) > 0) { // key < a[left]
				return left - 1;
			} else if (a[right].compareTo(key) < 0) { // key > a[right]
				return right + 1;
			}

		} else {
			E x = a[right]; // force exception if a is null or left or right is out of bounds
			x = a[left];

			if (a[mid].compareTo(key) == 0) { // key = a[mid]
				if (mid > 0) { // if key is more than once in the array => the smallest index 
					return search(a, key, left, mid - 1); 
				}
			} else if (a[mid].compareTo(key) > 0) { // a[mid] > key
				return search(a, key, left, mid - 1);

			} else {
				if (mid + 1 < a.length 
						&& a[mid + 1].compareTo(key) > 0 
						&& a[mid].compareTo(key) < 0) { // a[mid]< key < a[mid+1]
					mid += 1;
				} else { // a[mid] < key

					return search(a, key, mid + 1, right);
				}
			}

		}
		return mid;

	}

	public static void main(String[] args) {
		Integer[] a = { 1, 3, 5, 7, 9, 10 };
		BinarySearch bs = new BinarySearch();
		System.out.println(bs.search(a, 8));
	}

}
