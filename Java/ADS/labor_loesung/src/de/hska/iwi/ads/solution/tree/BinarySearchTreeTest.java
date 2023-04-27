package de.hska.iwi.ads.solution.tree;

import java.util.Map;

import de.hska.iwi.ads.dictionary.MapTest;

public class BinarySearchTreeTest extends MapTest {

	@Override
	public <K extends Comparable<K>, V> Map<K, V> createMap() {
		return new BinarySearchTree<K, V>();
	}

}
