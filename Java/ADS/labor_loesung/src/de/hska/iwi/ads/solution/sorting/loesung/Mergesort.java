package de.hska.iwi.ads.solution.sorting.loesung;

public class Mergesort<E extends Comparable<E>> extends BasisKlasseMergesort<E> {


	@Override
	protected void verschmelzen(E[] a, int left, int mid, int right) {
		int l = left;
		int r = mid + 1;
		for (int i = left; i <= right; i++) {
			if (r > right || (l <= mid && a[l].compareTo(a[r]) <= 0)) { // a[l] <= a[r]
				b[i] = a[l];
				l++;

			} else { // a[r]> a[l]
				b[i] = a[r];
				r++;
			}
		}
		
		// assign values to a
		for (int i = left; i <= right; i++) {
			a[i] =  b[i];
		}

	}
	public static void main(String[] args) {
		Integer[] a = { 2, 1, 7, 12, 9, 10, 8 };
		Mergesort<Integer> ms = new Mergesort<Integer> ();
		ms.sort(a);
		
	}

}
