package de.hska.iwi.ads.solution.sorting.loesung;

import de.hska.iwi.ads.sorting.AbstractMergesort;

public abstract class BasisKlasseMergesort<E extends Comparable<E>> extends AbstractMergesort<E> {
	
	@Override
	public void mergesort(E [] a, int left, int right) {
		
		
		
		if(left < right) {
			int mid = (left + right) / 2;
			mergesort(a, left , mid);
			mergesort(a, mid+1, right);
			verschmelzen(a, left, mid, right);
		}
		
	}
	protected abstract void verschmelzen(E [] a, int left, int middle, int right);  

}
