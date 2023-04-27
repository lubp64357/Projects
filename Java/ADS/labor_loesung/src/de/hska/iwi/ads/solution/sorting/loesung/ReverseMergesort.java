package de.hska.iwi.ads.solution.sorting.loesung;



public class ReverseMergesort<E extends Comparable<E>> extends BasisKlasseMergesort<E> {

	@Override
	protected void verschmelzen(E[] a, int left, int mid, int right) {
		// TODO Auto-generated method stub
		int l = left;
		int r = right; // strarting at the right end

		// reversing right half
		ReverseArray<E> rev = new ReverseArray<E>();
		rev.reverse(a, mid + 1, right);

		for (int i = left; i <= right; i++) {

			if ((r > right) || ((l <= mid) && a[l].compareTo(a[r]) <= 0)) {
				b[i] = a[l];
				l++;
			} else {
				b[i] = a[r];
				r--; // going left
			}
		}
		// assign values to a
		for (int i = left; i <= right; i++) {
			a[i] = b[i];
		}
	}
}
