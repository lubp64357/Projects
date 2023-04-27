package de.hska.iwi.ads.solution.hashtable;

import java.util.Map;

import de.hska.iwi.ads.dictionary.MapTest;

public class HashtableTest extends MapTest {

	@Override
	public <K extends Comparable<K>, V> Map<K, V> createMap() {
		return new Hashtable<K, V>(10);
	}

}